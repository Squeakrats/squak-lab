#pragma once
#include <functional>
#include <map>
#include <memory>
#include <squak/engine/RuntimeTypeInfo.h>
#include <squak/engine/rpc.h>
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

  void Invoke(RPC rpc) {
    this->GetRuntimeTypeInfo().methods[rpc.method](this, std::move(rpc.arguments));
  }

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