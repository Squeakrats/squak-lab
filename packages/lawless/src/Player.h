#pragma once
#include "Engine.h"
#include "SceneAsset.h"
#include <numbers>

class Player : public Actor {
public:
    using Super = Actor;

    Vector3 velocity{};
    bool canJump = true;

    Player(const ActorInitializer& initializer) : Actor(initializer) {
        this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);

        Vector3 force(engine.GetAxis("horizontal") * .0001f, -0.0006f, 0.0f);

        if (this->canJump && engine.GetKey(GLFW_KEY_SPACE)) {
            force.y += 0.005f;
            this->canJump = false;
        }

        this->velocity += deltaMs * force;

        Transform& transform = this->GetTransform();
        transform.position += deltaMs * velocity;

        if (force.x > 0) {
            transform.rotation.y = static_cast<float>(std::numbers::pi) / 2;
        }
        else if (force.x < 0) {
            transform.rotation.y = -static_cast<float>(std::numbers::pi) / 2;
        }

        velocity *= 0.90f;
        if (transform.position.y < 0) {
            transform.position.y = 0;
            this->velocity.y = 0;
            this->canJump = true;
        }
    }

    static const ActorCreatorEntry CREATORENTRY;
};