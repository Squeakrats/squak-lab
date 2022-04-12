#include "gltf.h"
#include <assert.h>
#include <fstream>

namespace gltf {

void AssertSystemIsLittleEndian() {
	uint32_t i = 0xFFFFFF00;
	assert(*reinterpret_cast<uint8_t*>(&i) == 0x00);
}

std::shared_ptr<SceneNode> Parse(const std::vector<uint8_t>& buffer) {
	// TODO - Remove assertion of little endian system
	AssertSystemIsLittleEndian();

	assert(buffer[0] == 'g');
	assert(buffer[1] == 'l');
	assert(buffer[2] == 'T');
	assert(buffer[3] == 'F');
	assert(*reinterpret_cast<const uint32_t*>(&buffer[4]) == 2);

	uint32_t chunkLength = *reinterpret_cast<const uint32_t*>(&buffer[12]);
	assert(*reinterpret_cast<const uint32_t*>(&buffer[16]) == gltf::binary::JSON);

	std::string source(buffer.begin() + 20, buffer.begin() + 20 + chunkLength);

	json::Object json = json::Parse(source);

	std::vector<std::shared_ptr<Geometry>> meshes{};
	for (json::Value& mesh : json["meshes"].get<json::Array>()) {
		meshes.push_back(std::make_shared<Geometry>(Geometry::CreatePlane(200, 200)));
	}

	std::vector<std::shared_ptr<SceneNode>> nodes{};
	for (json::Value& source : json["nodes"].get<json::Array>()) {
		std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
		node->name = source["name"].get<std::string>();

		// TODO - parse binary component of glb
		node->geometry = meshes[static_cast<size_t>(source["mesh"].get<double>())];

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

	// TODO - dont copy memory like this
	std::vector<uint8_t> buffer{};
	while (!file.eof()) { buffer.push_back(file.get()); }

	return Parse(buffer);
}

};
