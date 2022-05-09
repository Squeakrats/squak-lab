#pragma once
#include "Light.h"
#include "Mesh.h"
#include "Transform.h"
#include <memory>
#include <string>
#include <vector>

class SceneNode {
public:
  std::string name{};
  Transform transform;
  std::shared_ptr<Mesh> mesh{};
  std::shared_ptr<Light> light{};
  std::vector<std::shared_ptr<SceneNode>> children{};

  SceneNode()
    : transform() {}
  SceneNode(Transform transform)
    : transform(transform) {}
  void AddChild(std::shared_ptr<SceneNode> child) {
    this->children.push_back(child);
  }
};