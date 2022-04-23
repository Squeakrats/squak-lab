#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class Player : public Actor {
public:
    using Super = Actor;
    float speed = 0.0001f;

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        auto& transform = this->GetTransform();
        transform.rotation.y += deltaMs * speed;
    }

    static const ActorCreatorEntry CREATORENTRY;
};