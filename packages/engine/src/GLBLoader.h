#pragma once
#include "AssetManager.h"

class GLBLoader : public IAssetLoader {
public:
  virtual std::string GetType() override { return "glb"; }
  virtual std::shared_ptr<IAsset> Load(std::string name) override;
};