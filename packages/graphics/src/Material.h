#pragma once
#include <memory>
#include "Texture.h"
#include "Vector3.h"

struct Material {
	Vector3 baseColor{};
	std::shared_ptr<Texture> baseColorTexture{};
};