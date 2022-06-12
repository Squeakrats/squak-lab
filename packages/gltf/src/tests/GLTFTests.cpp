#include <gtest/gtest.h>
#include <fstream>
#include <squak/gltf.h>

TEST(GLTF, GLTFBasic) {
	// TODO - create an asset loading / path framework
	std::shared_ptr<SceneNode> scene = gltf::Load("..\\..\\..\\..\\..\\assets\\cube.glb");
	
	ASSERT_EQ(scene->name, "Scene");
	ASSERT_EQ(scene->children[0]->name, "Cube");
}
