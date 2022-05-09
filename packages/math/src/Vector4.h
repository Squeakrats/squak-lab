#pragma once
#include "Vector3.h"

class Vector4 {
public:
  float x, y, z, w;

  Vector4()
    : x(0)
    , y(0)
    , z(0)
    , w(0){};
  Vector4(float x, float y, float z, float w)
    : x(x)
    , y(y)
    , z(z)
    , w(w){};
  Vector4(Vector3 a, float w)
    : x(a.x)
    , y(a.y)
    , z(a.z)
    , w(w){};

  Vector3 xyz() { return Vector3(this->x, this->y, this->z); }
};
