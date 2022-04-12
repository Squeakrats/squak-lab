#include <gtest/gtest.h>
#include <fstream>
#include "gltf.h"

TEST(GLTF, GLTFBasic) {
	// TODO - create an asset loading / path framework
	json::Object json = gltf::Load("..\\..\\..\\..\\..\\assets\\cube.glb");
	ASSERT_EQ(json["meshes"][0]["name"].get<std::string>(), "Cube.001");
}
