#pragma once
#include "AssetManager.h"
#include "SceneNode.h"

struct SceneAsset : public IAsset {
  std::shared_ptr<SceneNode> scene{};

  SceneAsset(std::shared_ptr<SceneNode> scene)
    : scene(scene){};
};
