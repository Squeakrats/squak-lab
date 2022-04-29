#pragma once
#include "Matrix4.h"

class Transform {
public:
	Vector3 position;
	Vector3 rotation;

	Transform() : position(), rotation() {};
	Transform(Vector3 position) : position(position), rotation() {};

	Vector3 GetForward() {
		return (this->ToMatrix() * Vector4(0.0, 0.0, 1.0, 0.0)).xyz();
	}

	Vector3 GetRight() {
		return (this->ToMatrix() * Vector4(1.0, 0.0, 0.0, 0.0)).xyz();
	}

	Matrix4 ToMatrix() { return Matrix4::Create(this->position, this->rotation); }
};