#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class Player : public Actor {
public:
    using Super = Actor;

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        float side = engine.GetAxis("horizontal");
        float forward = engine.GetAxis("vertical");

        auto& transform = this->GetTransform();
        transform.position.x += side * .1f;
        transform.position.z -= forward * .1f;
    }

    static const ActorCreatorEntry CREATORENTRY;
};