#pragma once
#include <squak/engine/Engine.h>
#include <squak/graphics/SceneAsset.h>
#include <numbers>

class Player : public Actor {
  using Super = Actor;
public:
  using Super = Actor;
  float speed = 0.0001f;

  Player(const ActorInitializer& initializer)
    : Actor(initializer) {
    this->root->AddChild(engine.GetAsset<SceneAsset>("suzanne.glb")->scene);
  }

  virtual void Tick(float deltaMs) override {
    Super::Tick(deltaMs);
    auto& transform = this->GetTransform();
    transform.rotation.y += deltaMs * speed;
  }

  static RuntimeTypeInfo CreateRuntimeTypeInfo() {
    RuntimeTypeInfo info = Super::GetRuntimeTypeInfoInstance();
    info.id = "Player";
    info.create = [](const ActorInitializer& initializer) {
      return std::make_shared<Player>(initializer);
    };

    RuntimeTypeInfo::Register(info);

    return info;
  }

  static RuntimeTypeInfo& GetRuntimeTypeInfoInstance() {
    static RuntimeTypeInfo info = CreateRuntimeTypeInfo();

    return info;
  }
};
