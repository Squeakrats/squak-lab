#pragma once
#include "SceneNode.h"
#include "CameraNode.h"

struct IRenderer {
    virtual ~IRenderer() {}

    virtual void Render(CameraNode& camera, SceneNode& scene) = 0;
};