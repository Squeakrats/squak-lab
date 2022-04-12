#include "gltf.h"
#include <assert.h>
#include <fstream>

namespace gltf {

void AssertSystemIsLittleEndian() {
	uint32_t i = 0xFFFFFF00;
	assert(*reinterpret_cast<uint8_t*>(&i) == 0x00);
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

	std::shared_ptr<Geometry::Buffer> buffer = std::make_shared<Geometry::Buffer>();
	buffer->resize(binHeader[0]);
	source.read(reinterpret_cast<char*>(buffer->data()), buffer->size());
	assert(source.get() == EOF);

	std::vector<std::shared_ptr<Geometry::BufferView>> bufferViews{};
	for (json::Value& jsonView : json["bufferViews"].get<json::Array>()) {
		assert(jsonView["buffer"].get<double>() == 0.0);

		bufferViews.push_back(std::make_shared<Geometry::BufferView>(Geometry::BufferView{
			buffer,
			static_cast<size_t>(jsonView["byteOffset"].get<double>()),
			static_cast<size_t>(jsonView["byteLength"].get<double>())
		}));
	}

	std::vector<std::shared_ptr<Geometry::Accessor>> accessors{};
	for (json::Value& jsonView : json["accessors"].get<json::Array>()) {
		accessors.push_back(std::make_shared<Geometry::Accessor>(Geometry::Accessor{
			Geometry::AccessorType::Vector3,
			Geometry::ComponentType::Float,
			static_cast<size_t>(jsonView["count"].get<double>()),
			bufferViews[static_cast<size_t>(jsonView["bufferView"].get<double>())]
		}));
	}

	std::vector<std::shared_ptr<Geometry>> meshes{};
	for (json::Value& mesh : json["meshes"].get<json::Array>()) {
		json::Array& primitives = mesh["primitives"].get<json::Array>();
		assert(primitives.size() == 1);

		json::Object& primitive = primitives[0].get<json::Object>();

		Geometry::Attributes attributes{};
		for (auto& attribute : primitive["attributes"].get<json::Object>().entries) {
			if (attribute.first != "POSITION") {
				continue;
			}

			attributes.insert(std::make_pair(
				Geometry::AttributeType::Position,
				accessors[static_cast<size_t>(attribute.second.get<double>())]
			));
		}

		std::shared_ptr<Geometry::Accessor> indices = accessors[static_cast<size_t>(primitive["indices"].get<double>())];
		meshes.push_back(std::make_shared<Geometry>(std::move(attributes), indices));
	}

	std::vector<std::shared_ptr<SceneNode>> nodes{};
	for (json::Value& source : json["nodes"].get<json::Array>()) {
		json::Object& object = source.get<json::Object>();

		std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
		node->name = object["name"].get<std::string>();

		if (object.entries.find("mesh") != object.entries.end()) {
			node->geometry = meshes[static_cast<size_t>(source["mesh"].get<double>())];
		}
		
		nodes.push_back(node);
	}

	json::Value& scene = json["scenes"][0];

	std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>();
	root->name = scene["name"].get<std::string>();

	for (json::Value& source : scene["nodes"].get<json::Array>()) {
		size_t node = static_cast<size_t>(source.get<double>());
		root->children.push_back(nodes[node]);
	}

	return root;
}

std::shared_ptr<SceneNode> Load(std::string path) {
	std::ifstream file(path, std::ios::binary);

	return Parse(file);
}

};
