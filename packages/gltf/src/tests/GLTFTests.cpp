#include <gtest/gtest.h>
#include <fstream>
#include "gltf.h"

TEST(GLTF, GLTFBasic) {
	// TODO - create an asset loading / path framework
	std::ifstream file("..\\..\\..\\..\\..\\assets\\cube.glb", std::ios::binary);
	std::vector<uint8_t> buffer{};

	// TODO - dont copy memory like this
	while (!file.eof()) { buffer.push_back(file.get()); }

	json::Object json = gltf::Parse(buffer);
	
	ASSERT_EQ(json["meshes"][0]["name"].get<std::string>(), "Cube.001");
}
