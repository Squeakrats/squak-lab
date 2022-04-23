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
    float turnSpeed = 0.001f;
    float moveSpeed = 0.01f;

    PlayerCamera(const ActorInitializer& initializer) : Actor(initializer) {
        this->root = this->camera;
        this->root->transform.position.z = 10;

        // make the camera active
        engine.SetCamera(this->camera);
    }

    virtual void Tick(float deltaMs) override {
        auto& transform = this->GetTransform();
        Vector3 forward = transform.GetForward();

        transform.position += -1 * deltaMs * this->moveSpeed * engine.GetAxis("Forward") * forward;
        transform.rotation.y += deltaMs * this->turnSpeed * engine.GetAxis("Turn");
        transform.rotation.x += deltaMs * this->turnSpeed * engine.GetAxis("Up");

    }

    static const ActorCreatorEntry CREATORENTRY;
};