#pragma once
#include "SceneNode.h"

class CameraNode : public SceneNode {
public:
	Matrix4 perspective;

	CameraNode(Matrix4 perspective) : perspective(perspective) {}
};