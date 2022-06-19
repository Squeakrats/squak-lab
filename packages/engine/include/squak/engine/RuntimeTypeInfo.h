#pragma once
#include <functional>
#include <map>
#include <string>
#include <memory>

class Actor;
class Engine;
class Transform;

class ActorInitializer {
public:
  std::string id;
  Transform& transform;
  Engine& engine;
};

struct RuntimeTypeInfo {
  std::string id{};
  std::map<std::string, std::function<void(void* t)>> methods{};
  std::function<std::shared_ptr<Actor>(ActorInitializer&)> create{};

  static std::map<std::string, RuntimeTypeInfo>& GetTypes() {
    static std::map<std::string, RuntimeTypeInfo> types{};

    return types;
  }

  static void Register(RuntimeTypeInfo info) { GetTypes().insert(std::make_pair(info.id, info)); }
};