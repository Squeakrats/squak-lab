#pragma once
#include "Vector3.h"

class Light {
public:
  Vector3 color;

  Light(Vector3 color) : color(color) {}
};