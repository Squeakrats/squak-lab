#include <squak/gltf.h>
#include <squak/graphics/SceneAsset.h>
#include "utility.h"
#include <fstream>

namespace gltf {

void AssertSystemIsLittleEndian() {
  uint32_t i = 0xFFFFFF00;
  Assert(*reinterpret_cast<uint8_t*>(&i) == 0x00, "big endian is unsupported");
}

BufferAccessor::Type ConvertAccessorType(const std::string& type) {
  static std::map<std::string, BufferAccessor::Type> table = {
    { "VEC2", BufferAccessor::Type::Vector2 },
    { "VEC3", BufferAccessor::Type::Vector3 },
    { "VEC4", BufferAccessor::Type::Vector4 },
    { "SCALAR", BufferAccessor::Type::Scalar },
  };

  return table.at(type);
}

BufferAccessor::ComponentType ConvertComponentType(uint32_t type) {
  static std::map<uint32_t, BufferAccessor::ComponentType> table = {
    { 5123, BufferAccessor::ComponentType::UnsignedShort },
    { 5125, BufferAccessor::ComponentType::UnsignedInt },
    { 5126, BufferAccessor::ComponentType::Float },
  };

  return table.at(type);
}

std::shared_ptr<BufferView> ConvertBufferView(json::Object& source,
                                              std::shared_ptr<Buffer> buffer) {
  Assert(source["buffer"].get<double>() == 0.0,
         "only one glb buffer is supported");

  return std::make_shared<BufferView>(buffer,
                                      source["byteOffset"].as<size_t>(),
                                      source["byteLength"].as<size_t>());
}

std::shared_ptr<BufferAccessor> ConvertBufferAccessor(
  json::Object& source,
  std::vector<std::shared_ptr<BufferView>>& bufferViews) {
  return std::make_shared<BufferAccessor>(
    ConvertAccessorType(source["type"].get<std::string>()),
    ConvertComponentType(source["componentType"].as<uint32_t>()),
    source["count"].as<size_t>(),
    bufferViews[source["bufferView"].as<size_t>()]);
}

std::pair<std::string, std::shared_ptr<BufferView>> ConvertImage(
  json::Object& source,
  std::vector<std::shared_ptr<BufferView>>& bufferViews) {
  return std::make_pair(source["mimeType"].get<std::string>(),
                        bufferViews[source["bufferView"].as<size_t>()]);
}

std::shared_ptr<Texture> ConvertTexture(
  json::Object& source,
  std::vector<std::pair<std::string, std::shared_ptr<BufferView>>>& images) {
  auto& image = images[source["source"].as<size_t>()];

  return std::make_shared<Texture>(image.first, image.second);
}

std::shared_ptr<Material> ConvertMaterial(
  json::Object& source,
  std::vector<std::shared_ptr<Texture>>& textures) {
  std::shared_ptr<Material> material = std::make_shared<Material>();

  auto& pbrMetallicRoughness =
    source["pbrMetallicRoughness"].get<json::Object>();
  if (pbrMetallicRoughness.has("baseColorTexture")) {
    material->baseColorTexture =
      textures[pbrMetallicRoughness["baseColorTexture"]["index"].as<size_t>()];
  } else if (pbrMetallicRoughness.has("baseColorFactor")) {
    auto& colorFactor =
      pbrMetallicRoughness["baseColorFactor"].get<json::Array>();
    material->baseColor.x = colorFactor[0].as<float>();
    material->baseColor.y = colorFactor[1].as<float>();
    material->baseColor.z = colorFactor[2].as<float>();
  } else {
    material->baseColor = Vector3(1, 0, 0);
  }

  return material;
}

Mesh::Geometry ConvertPrimitive(
  json::Object& source,
  std::vector<std::shared_ptr<BufferAccessor>>& accessors,
  std::vector<std::shared_ptr<Material>>& materials) {
  Mesh::Geometry geometry{};

  geometry.indices = accessors[source["indices"].as<size_t>()];

  if (source.has("material")) {
    geometry.material = materials[source["material"].as<size_t>()];
  }

  for (auto& attribute : source["attributes"].get<json::Object>().entries) {
    if (attribute.first == "POSITION") {
      geometry.positions = accessors[attribute.second.as<size_t>()];
    } else if (attribute.first == "NORMAL") {
      geometry.normals = accessors[attribute.second.as<size_t>()];
    } else if (attribute.first == "TEXCOORD_0") {
      geometry.textureCoordinatess = accessors[attribute.second.as<size_t>()];
    } else if (attribute.first == "COLOR_0") {
      geometry.colors = accessors[attribute.second.as<size_t>()];
    }
  }

  return geometry;
}

std::shared_ptr<Mesh> ConvertMesh(
  json::Object& source,
  std::vector<std::shared_ptr<BufferAccessor>>& accessors,
  std::vector<std::shared_ptr<Material>>& materials) {
  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
  for (json::Value& primitive : source["primitives"].get<json::Array>()) {
    mesh->geometries.push_back(
      ConvertPrimitive(primitive.get<json::Object>(), accessors, materials));
  }

  return mesh;
}

std::shared_ptr<SceneNode> ConvertNode(
  json::Object& source,
  std::vector<std::shared_ptr<Mesh>>& meshes) {
  std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();

  node->name = source["name"].get<std::string>();

  if (source.has("mesh")) {
    node->mesh = meshes[source["mesh"].as<size_t>()];
  }

  return node;
}

std::shared_ptr<SceneNode> ConvertScene(
  json::Object& source,
  std::vector<std::shared_ptr<SceneNode>>& nodes) {
  std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>();

  scene->name = source["name"].get<std::string>();

  for (json::Value& node : source["nodes"].get<json::Array>()) {
    scene->children.push_back(nodes[node.as<size_t>()]);
  }

  return scene;
}

std::shared_ptr<SceneNode> Parse(std::ifstream& source) {
  // TODO - Remove assertion of little endian system
  AssertSystemIsLittleEndian();

  uint32_t gltfHeader[3] = { 0 };
  source.read(reinterpret_cast<char*>(gltfHeader), sizeof(gltfHeader));
  Assert(gltfHeader[0] == gltf::binary::GLTF, "invalid gltf magic");
  Assert(gltfHeader[1] == 2, "invalid gltf version");

  uint32_t jsonHeader[2] = { 0 };
  source.read(reinterpret_cast<char*>(jsonHeader), sizeof(jsonHeader));
  Assert(jsonHeader[1] == gltf::binary::JSON, "invalid chunk type");

  std::string jsonData{};
  jsonData.resize(jsonHeader[0]);
  source.read(jsonData.data(), jsonData.size());

  json::Object json = json::Parse(jsonData);

  uint32_t binHeader[2] = { 0 };
  source.read(reinterpret_cast<char*>(binHeader), sizeof(binHeader));
  Assert(binHeader[1] == gltf::binary::BIN, "invalid chunk type");

  std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(binHeader[0]);
  source.read(reinterpret_cast<char*>(buffer->data), buffer->size);
  Assert(source.get() == EOF, "unhandled data in buffer");

  std::vector<std::shared_ptr<BufferView>> bufferViews{};
  for (json::Value& bufferView : json["bufferViews"].get<json::Array>()) {
    bufferViews.push_back(
      ConvertBufferView(bufferView.get<json::Object>(), buffer));
  }

  std::vector<std::shared_ptr<BufferAccessor>> accessors{};
  for (json::Value& accessor : json["accessors"].get<json::Array>()) {
    accessors.push_back(
      ConvertBufferAccessor(accessor.get<json::Object>(), bufferViews));
  }

  std::vector<std::pair<std::string, std::shared_ptr<BufferView>>> images{};
  if (json.has("images")) {
    for (json::Value& image : json["images"].get<json::Array>()) {
      images.push_back(ConvertImage(image.get<json::Object>(), bufferViews));
    }
  }

  std::vector<std::shared_ptr<Texture>> textures{};
  if (json.has("textures")) {
    for (json::Value& texture : json["textures"].get<json::Array>()) {
      textures.push_back(ConvertTexture(texture.get<json::Object>(), images));
    }
  }

  std::vector<std::shared_ptr<Material>> materials{};
  if (json.has("materials")) {
    for (json::Value& material : json["materials"].get<json::Array>()) {
      materials.push_back(
        ConvertMaterial(material.get<json::Object>(), textures));
    }
  }

  std::vector<std::shared_ptr<Mesh>> meshes{};
  for (json::Value& mesh : json["meshes"].get<json::Array>()) {
    meshes.push_back(
      ConvertMesh(mesh.get<json::Object>(), accessors, materials));
  }

  std::vector<std::shared_ptr<SceneNode>> nodes{};
  for (json::Value& source : json["nodes"].get<json::Array>()) {
    nodes.push_back(ConvertNode(source.get<json::Object>(), meshes));
  }

  return ConvertScene(json["scenes"][0].get<json::Object>(), nodes);
}

std::shared_ptr<SceneNode> Load(std::string path) {
  std::ifstream file(path, std::ios::binary);
  Assert(file.is_open(), "file must be valid to read");

  return Parse(file);
}

};
