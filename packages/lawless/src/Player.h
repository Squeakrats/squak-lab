#pragma once
#include "Engine.h"
#include "SceneAsset.h"

class Player : public Actor {
public:
    using Super = Actor;

    const static float DEFAULT_SPEED;

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        this->GetTransform().rotation.y += .001f * deltaMs;

        Vector3 movement(0, 0, 0);
        if (engine.GetKey(GLFW_KEY_W)) {
            movement.z -= .01f * deltaMs;
        }
        else if (engine.GetKey(GLFW_KEY_S)) {
            movement.z += .01f * deltaMs;
        }

        this->GetTransform().position += DEFAULT_SPEED * movement.normalize();
    }

    static const ActorCreatorEntry CREATORENTRY;
};