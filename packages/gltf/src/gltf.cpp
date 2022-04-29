#include "gltf.h"
#include "utility.h"
#include <fstream>
#include "SceneAsset.h"

namespace gltf {

void AssertSystemIsLittleEndian() {
	uint32_t i = 0xFFFFFF00;
	assert(*reinterpret_cast<uint8_t*>(&i) == 0x00);
}

BufferAccessor::Type ConvertAccessorType(const std::string& type) {
	static std::map<std::string, BufferAccessor::Type> table = {
		{ "VEC2",  BufferAccessor::Type::Vector2 },
		{ "VEC3",  BufferAccessor::Type::Vector3 },
		{ "VEC4",  BufferAccessor::Type::Vector4 },
		{ "SCALAR",  BufferAccessor::Type::Scalar },
	};

	return table.at(type);
}

BufferAccessor::ComponentType ConvertComponentType(uint32_t type) {
	static std::map<uint32_t, BufferAccessor::ComponentType> table = {
		{ 5123,  BufferAccessor::ComponentType::UnsignedShort },
		{ 5125,  BufferAccessor::ComponentType::UnsignedInt },
		{ 5126,  BufferAccessor::ComponentType::Float },
	};

	return table.at(type);
}

std::shared_ptr<SceneNode> Parse(std::ifstream& source) {
	// TODO - Remove assertion of little endian system
	AssertSystemIsLittleEndian();

	uint32_t gltfHeader[3] = { 0 };
	source.read(reinterpret_cast<char*>(gltfHeader), sizeof(gltfHeader));
	assert(gltfHeader[0] == gltf::binary::GLTF);
	assert(gltfHeader[1] == 2);

	uint32_t jsonHeader[2] = { 0 };
	source.read(reinterpret_cast<char*>(jsonHeader), sizeof(jsonHeader));
	assert(jsonHeader[1] == gltf::binary::JSON);

	std::string jsonData{};
	jsonData.resize(jsonHeader[0]);
	source.read(jsonData.data(), jsonData.size());

	json::Object json = json::Parse(jsonData);

	uint32_t binHeader[2] = { 0 };
	source.read(reinterpret_cast<char*>(binHeader), sizeof(binHeader));
	assert(binHeader[1] == gltf::binary::BIN);

	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(binHeader[0]);
	source.read(reinterpret_cast<char*>(buffer->data), buffer->size);
	assert(source.get() == EOF);

	std::vector<std::shared_ptr<BufferView>> bufferViews{};
	for (json::Value& jsonView : json["bufferViews"].get<json::Array>()) {
		assert(jsonView["buffer"].get<double>() == 0.0);

		bufferViews.push_back(std::make_shared<BufferView>(BufferView{
			buffer,
			jsonView["byteOffset"].as<size_t>(),
			jsonView["byteLength"].as<size_t>()
		}));
	}

	std::vector<std::shared_ptr<BufferAccessor>> accessors{};
	for (json::Value& jsonView : json["accessors"].get<json::Array>()) {
		accessors.push_back(std::make_shared<BufferAccessor>(BufferAccessor{
			ConvertAccessorType(jsonView["type"].get<std::string>()),
			ConvertComponentType(jsonView["componentType"].as<uint32_t>()),
			jsonView["count"].as<size_t>(),
			bufferViews[jsonView["bufferView"].as<size_t>()]
		}));
	}

	std::vector<std::pair<std::string, std::shared_ptr<BufferView>>> images{};
	if (json.entries.find("images") != json.entries.end()) {
		for (json::Value& image : json["images"].get<json::Array>()) {
			images.push_back(std::make_pair(
				image["mimeType"].get<std::string>(),
				bufferViews[image["bufferView"].as<size_t>()]
			));
		}
	}

	std::vector<std::shared_ptr<Texture>> textures{};
	if (json.entries.find("textures") != json.entries.end()) {
		for (json::Value& texture : json["textures"].get<json::Array>()) {
			auto& source = images[texture["source"].as<size_t>()];
			textures.push_back(std::make_shared<Texture>(
				source.first,
				source.second
			));
		}
	}

	std::vector<std::shared_ptr<Material>> materials{};
	if (json.entries.find("materials") != json.entries.end()) {
		for (json::Value& jsonMaterial : json["materials"].get<json::Array>()) {
			std::shared_ptr<Material> material = std::make_shared<Material>();

			auto& pbrMetallicRoughness = jsonMaterial["pbrMetallicRoughness"].get<json::Object>();
			if (pbrMetallicRoughness.entries.find("baseColorTexture") != pbrMetallicRoughness.entries.end()) {
				material->baseColorTexture = textures[static_cast<size_t>(pbrMetallicRoughness["baseColorTexture"]["index"].get<double>())];
			}
			else if (pbrMetallicRoughness.entries.find("baseColorFactor") != pbrMetallicRoughness.entries.end()) {
				auto& colorFactor = pbrMetallicRoughness["baseColorFactor"].get<json::Array>();
				material->baseColor.x = colorFactor[0].as<float>();
				material->baseColor.y = colorFactor[1].as<float>();
				material->baseColor.z = colorFactor[2].as<float>();
			} else {
				material->baseColor = Vector3(1, 0, 0);
			}
			
			materials.push_back(material);
		}
	}

	std::vector<std::shared_ptr<Mesh>> meshes{};
	for (json::Value& mesh : json["meshes"].get<json::Array>()) {
		std::vector<Mesh::Geometry> geometries{};
		for (json::Value& primitiveValue : mesh["primitives"].get<json::Array>()) {
			json::Object& primitive = primitiveValue.get<json::Object>();

			Mesh::Geometry geometry{};

			geometry.indices = accessors[primitive["indices"].as<size_t>()];

			if (primitive.entries.find("material") != primitive.entries.end()) {
				geometry.material = materials[primitive["material"].as<size_t>()];
			}

			for (auto& attribute : primitive["attributes"].get<json::Object>().entries) {
				if (attribute.first == "POSITION") {
					geometry.positions = accessors[attribute.second.as<size_t>()];
				}
				else if (attribute.first == "NORMAL") {
					geometry.normals = accessors[attribute.second.as<size_t>()];
				}
				else if (attribute.first == "TEXCOORD_0") {
					geometry.textureCoordinatess = accessors[attribute.second.as<size_t>()];
				}
				else if (attribute.first == "COLOR_0") {
					geometry.colors = accessors[attribute.second.as<size_t>()];
				}
			}

			geometries.push_back(std::move(geometry));
		}

		meshes.push_back(std::make_shared<Mesh>(std::move(geometries)));
	}

	std::vector<std::shared_ptr<SceneNode>> nodes{};
	for (json::Value& source : json["nodes"].get<json::Array>()) {
		json::Object& object = source.get<json::Object>();

		std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
		node->name = object["name"].get<std::string>();

		if (object.entries.find("mesh") != object.entries.end()) {
			node->mesh = meshes[source["mesh"].as<size_t>()];
		}
		
		nodes.push_back(node);
	}

	json::Value& scene = json["scenes"][0];

	std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>();
	root->name = scene["name"].get<std::string>();

	for (json::Value& source : scene["nodes"].get<json::Array>()) {
		root->children.push_back(nodes[source.as<size_t>()]);
	}

	return root;
}

std::shared_ptr<SceneNode> Load(std::string path) {
	std::ifstream file(path, std::ios::binary);
	Assert(file.is_open(), "file must be valid to read");

	return Parse(file);
}

};
