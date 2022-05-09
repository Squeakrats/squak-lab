#pragma once
#include "SceneNode.h"
class CameraNode : public SceneNode {
public:
  Matrix4 perspective;

  Matrix4 GetView() {
    return Matrix4::RotateX(-1 * this->transform.rotation.x) *
           Matrix4::RotateY(-1 * this->transform.rotation.y) *
           Matrix4::RotateZ(-1 * this->transform.rotation.z) *
           Matrix4::Translate(-1 * this->transform.position);
  }

  CameraNode(Matrix4 perspective)
    : perspective(perspective) {}
};