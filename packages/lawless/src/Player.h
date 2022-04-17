#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class Player : public Actor {
public:
    using Super = Actor;

    float walkSpeed = .00004f;
    Vector3 velocity{};

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        Transform& transform = this->GetTransform();

        Vector3 force(engine.GetAxis("horizontal"), engine.GetAxis("vertical"), 0.0f);
        this->velocity += deltaMs * walkSpeed * force.normalize();
        transform.position += deltaMs * velocity;
        this->velocity *= 0.9f;

        transform.rotation.y += 0.001 * deltaMs;
    }

    static const ActorCreatorEntry CREATORENTRY;
};