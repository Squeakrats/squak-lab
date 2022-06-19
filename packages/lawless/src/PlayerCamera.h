#pragma once
#include "Player.h"
#include <iostream>
#include <numbers>
#include <squak/engine/Engine.h>
#include <squak/graphics/SceneAsset.h>

class PlayerCamera : public Actor {
  using Super = Actor;

public:
  using Super = Actor;

  std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(
    Matrix4::Perspective(static_cast<float>(std::numbers::pi) / 8.0f,
                         .01f,
                         100));
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
    transform.position += -1 * deltaMs * this->moveSpeed *
                          engine.GetAxis("Forward") * transform.GetForward();
    transform.position += -1 * deltaMs * this->moveSpeed *
                          engine.GetAxis("Strafe") * transform.GetRight();
    transform.rotation.y += deltaMs * this->turnSpeed * engine.GetAxis("Turn");
    transform.rotation.x += deltaMs * this->turnSpeed * engine.GetAxis("Up");
  }

  void MoveRight() { this->GetTransform().position.x += 0.1; }
  void MoveLeft() { this->GetTransform().position.x -= 0.1; }
  void MoveForward() { this->GetTransform().position.z -= 0.1; }
  void MoveBack() { this->GetTransform().position.z += 0.1; }

  virtual RuntimeTypeInfo& GetRuntimeTypeInfo() override {
    return GetRuntimeTypeInfoInstance();
  }

  static RuntimeTypeInfo CreateRuntimeTypeInfo() {
    RuntimeTypeInfo info = Super::GetRuntimeTypeInfoInstance();
    info.id = "PlayerCamera";

    info.methods.insert(std::make_pair("MoveRight", [](void* t) {
      static_cast<PlayerCamera*>(t)->MoveRight();
    }));
    info.methods.insert(std::make_pair(
      "MoveLeft", [](void* t) { static_cast<PlayerCamera*>(t)->MoveLeft(); }));
    info.methods.insert(std::make_pair("MoveForward", [](void* t) {
      static_cast<PlayerCamera*>(t)->MoveForward();
    }));
    info.methods.insert(std::make_pair(
      "MoveBack", [](void* t) { static_cast<PlayerCamera*>(t)->MoveBack(); }));

    info.create = [](const ActorInitializer& initializer) {
      return std::make_shared<PlayerCamera>(initializer);
    };

    RuntimeTypeInfo::Register(info);

    return info;
  }

  static RuntimeTypeInfo& GetRuntimeTypeInfoInstance() {
    static RuntimeTypeInfo info = CreateRuntimeTypeInfo();

    return info;
  }
};
