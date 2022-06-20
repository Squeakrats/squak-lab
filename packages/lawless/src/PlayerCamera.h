#pragma once
#include "PlayerCamera.generated.h"
#include "Player.h"
#include "utility.h"
#include <iostream>
#include <numbers>
#include <squak/engine/Engine.h>
#include <squak/graphics/SceneAsset.h>

SCLASS()
class PlayerCamera : public Actor {
  SCLASS_PLAYERCAMERA_GENERATED_HEAD
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

  SFUNCTION()
  void MoveRight();
  void MoveRight_Implementation() { this->GetTransform().position.x += 0.1; }

  SFUNCTION()
  void MoveLeft();
  void MoveLeft_Implementation() { this->GetTransform().position.x -= 0.1; }

  SFUNCTION()
  void MoveForward();
  void MoveForward_Implementation() { this->GetTransform().position.z -= 0.1; }

  SFUNCTION()
  void MoveBack();
  void MoveBack_Implementation() { this->GetTransform().position.z += 0.1; }

  SFUNCTION()
  void LogMessage(std::string message);
  void LogMessage_Implementation(std::string message) { Log(message.c_str()); }
};

SCLASS_PLAYERCAMERA_GENERATED_BODY