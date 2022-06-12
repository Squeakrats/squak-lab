#include <squak/core/AssetManager.h>

std::shared_ptr<IAsset> AssetManager::GetAsset(std::string name) {
  auto find = this->assets.find(name);
  if (find != this->assets.end()) {
    return find->second;
  }

  std::string type = name.substr(name.find_last_of(".") + 1);
  std::shared_ptr<IAsset> asset =
    this->loaders.at(type)->Load(this->basePath + name);
  this->assets.insert(std::make_pair(name, asset));

  return asset;
}