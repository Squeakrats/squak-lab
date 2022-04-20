#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>
#include <iostream>
#include "Player.h"

class PlayerCamera : public Actor {
public:
    using Super = Actor;

    std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(Matrix4::Perspective(static_cast<float>(std::numbers::pi) / 8.0f, 1, 100));
    std::shared_ptr<Player> target{};

    PlayerCamera(const ActorInitializer& initializer) : Actor(initializer) {
        this->root = this->camera;
        this->camera->transform.position.z = 15;
        this->camera->transform.position.y = 7;
        this->camera->transform.rotation.x = -static_cast<float>(std::numbers::pi) / 8.0;
        
        // make the camera active
        engine.SetCamera(this->camera);
    }

    virtual void Tick(float deltaMs) override {
        Vector3 targetPosition = this->target->GetRoot()->transform.position + Vector3(0.0f, 7.0f, 15.0f);
        Vector3 delta = deltaMs * .003 * (targetPosition - this->GetTransform().position);

        this->GetTransform().position += delta;
    }

    

    static const ActorCreatorEntry CREATORENTRY;
};