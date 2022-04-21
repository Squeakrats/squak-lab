#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class Player : public Actor {
public:
    using Super = Actor;
    float speed = 0.01f;

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        auto& transform = this->GetTransform();

        Vector3 input(engine.GetAxis("Strafe"), 0, engine.GetAxis("Forward"));
        Vector3 direction = (this->GetTransform().ToMatrix() * Vector4(input, 0.0)).xyz();
        if (direction.length() > 0) {
            transform.position += deltaMs * this->speed * direction.normalize();
        }
    }

    static const ActorCreatorEntry CREATORENTRY;
};