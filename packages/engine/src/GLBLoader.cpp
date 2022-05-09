#include "GLBLoader.h"
#include "SceneAsset.h"
#include "gltf.h"

std::shared_ptr<IAsset> GLBLoader::Load(std::string name) {
  return std::make_shared<SceneAsset>(gltf::Load(name));
}