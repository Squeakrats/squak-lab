#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Matrix4.h"
#include "Geometry.h"

class SceneNode {
public:
	std::string name{};
	Matrix4 transform = Matrix4::Identity();
	std::shared_ptr<Geometry> geometry{};
	std::vector<std::shared_ptr<SceneNode>> children{};
};