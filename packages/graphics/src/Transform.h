#pragma once
#include "Matrix4.h"

class Transform {
public:
	Vector3 position;
	Vector3 rotation;

	Transform() : position(), rotation() {};

	Matrix4 ToMatrix() { return Matrix4::Create(this->position, this->rotation); }
};