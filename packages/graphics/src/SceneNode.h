#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Transform.h"
#include "Geometry.h"

class SceneNode {
public:
	std::string name{};
	Transform transform{};
	std::shared_ptr<Geometry> geometry{};
	std::vector<std::shared_ptr<SceneNode>> children{};
};