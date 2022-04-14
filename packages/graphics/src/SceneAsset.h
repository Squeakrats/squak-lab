#pragma once
#include "SceneNode.h"
#include "AssetManager.h"

struct SceneAsset : public IAsset {
	std::shared_ptr<SceneNode> scene{};

	SceneAsset(std::shared_ptr<SceneNode> scene) : scene(scene) {};
};
