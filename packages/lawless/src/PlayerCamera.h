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
    float turnSpeed = 0.005;

    PlayerCamera(const ActorInitializer& initializer) : Actor(initializer) {
        this->root = this->camera;

        // make the camera active
        engine.SetCamera(this->camera);
    }

    virtual void Tick(float deltaMs) override {
        auto& transform = this->GetTransform();
        auto& targetTransform = this->target->GetTransform();

        float turn = engine.GetAxis("Turn");
        if (turn != 0) {
            targetTransform.rotation.y += deltaMs * this->turnSpeed * turn;
        }

        transform.position = targetTransform.position
            + -10.0 * targetTransform.GetForward()
            + Vector3(0.0, 3, 0.0);

        this->camera->transform.rotation.y = static_cast<float>(std::numbers::pi) + targetTransform.rotation.y;
    }

    static const ActorCreatorEntry CREATORENTRY;
};