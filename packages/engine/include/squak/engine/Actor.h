#pragma once
#include <squak/engine/RuntimeTypeInfo.h>
#include <functional>
#include <map>
#include <memory>
#include <string>

class SceneNode;
class ActorInitializer;
class Transform;
class Engine;

class Actor {
protected:
  std::string id;
  std::shared_ptr<SceneNode> root;
  Engine& engine;

public:
  Actor(const ActorInitializer& initializer);
  virtual ~Actor(){};

  std::shared_ptr<SceneNode> GetRoot() { return root; }
  Transform& GetTransform();

  virtual void Tick(float deltaMs);

  virtual RuntimeTypeInfo& GetRuntimeTypeInfo() {
    return GetRuntimeTypeInfoInstance();
  }

  static RuntimeTypeInfo CreateRuntimeTypeInfo() {
    RuntimeTypeInfo info{};

    return info;
  }

  static RuntimeTypeInfo& GetRuntimeTypeInfoInstance() {
    static RuntimeTypeInfo info = CreateRuntimeTypeInfo();

    return info;
  }
};