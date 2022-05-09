#pragma once
#include "CameraNode.h"
#include "SceneNode.h"

struct IRenderer {
  virtual ~IRenderer() {}

  virtual void Render(CameraNode& camera, SceneNode& scene) = 0;
};