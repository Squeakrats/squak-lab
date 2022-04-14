#pragma once
#include "SceneNode.h"

struct IRenderer {
    virtual void Render(Matrix4& camera, SceneNode& scene) = 0;
};