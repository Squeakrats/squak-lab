#pragma once
#include <memory>
#include "Matrix4.h"
#include "Geometry.h"

class SceneNode {
public:
	Matrix4 transform;
	std::shared_ptr<Geometry> geometry{};
};