#include <squak/engine/GLBLoader.h>
#include <squak/graphics/SceneAsset.h>
#include <squak/gltf.h>

std::shared_ptr<IAsset> GLBLoader::Load(std::string name) {
  return std::make_shared<SceneAsset>(gltf::Load(name));
}