#pragma once
#include <memory>
#include "Texture.h"

struct Material {
	std::shared_ptr<Texture> baseColorTexture{};
};