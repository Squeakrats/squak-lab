#pragma once
#include "Texture.h"
#include "Vector3.h"
#include <memory>

struct Material {
  Vector3 baseColor{};
  std::shared_ptr<Texture> baseColorTexture{};
};