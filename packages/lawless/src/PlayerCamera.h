#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class PlayerCamera : public Actor {
public:
    using Super = Actor;

    std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(Matrix4::Perspective(110.0f, 1, 100));

    PlayerCamera(const ActorInitializer& initializer) : Actor(initializer) {
        this->root = this->camera;
        this->camera->transform.position.z = 70.0f;
        
        // make the camera active
        engine.SetCamera(this->camera);
    }

    static const ActorCreatorEntry CREATORENTRY;
};