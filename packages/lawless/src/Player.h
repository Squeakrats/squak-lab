#pragma once
#include "Player.generated.h"
#include <squak/engine/Engine.h>
#include <squak/graphics/SceneAsset.h>
#include <numbers>

SCLASS()
class Player : public Actor {
  SCLASS_PLAYER_GENERATED_HEAD
public:
  using Super = Actor;
  float speed = 0.0001f;

  Player(const ActorInitializer& initializer)
    : Actor(initializer) {
    this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
    this->GetTransform().position.y = 2;
  }

  virtual void Tick(float deltaMs) override {
    Super::Tick(deltaMs);
    auto& transform = this->GetTransform();
    transform.rotation.y += deltaMs * speed;
  }
};

SCLASS_PLAYER_GENERATED_BODY