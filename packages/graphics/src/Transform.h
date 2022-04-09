#pragma once
#include "Vector3.h"

class Transform {
public:
	Vector3 position;

	Transform() : position() {};
	Transform(Vector3 position) : position(position) {};
};