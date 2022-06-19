#pragma once
#include <functional>
#include <map>
#include <memory>
#include <squak/engine/rpc.h>
#include <string>
#include <utility>

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
  std::map<std::string,
           std::function<void(void* t, std::vector<RPC::Argument>&&)>>
    methods{};
  std::function<std::shared_ptr<Actor>(ActorInitializer&)> create{};

  template<typename Class, typename... Args, size_t... Indices>
  void InvokeHelper(Class* self,
                    void (Class::*impl)(Args...),
                    std::vector<RPC::Argument>&& args,

                    std::integer_sequence<size_t, Indices...>) {
    (self->*impl)((std::get<Args>(args.at(Indices)))...);
  }

  template<typename Class, typename... Args>
  void Invoke(Class* self,
              std::vector<RPC::Argument>&& args,
              void (Class::*impl)(Args...)) {
    this->InvokeHelper(
      self, impl, std::move(args), std::index_sequence_for<Args...>{});
  }

  template<typename Class, typename... Args>
  void RegisterMethod(std::string&& name, void (Class::*impl)(Args...)) {
    this->methods.insert(std::make_pair(
      name, // todo - dont mutable, just cast
      [this, impl](void* self, std::vector<RPC::Argument>&& args) mutable {
        this->Invoke(static_cast<Class*>(self), std::move(args), impl);
      }));
  }

  static std::map<std::string, RuntimeTypeInfo>& GetTypes() {
    static std::map<std::string, RuntimeTypeInfo> types{};

    return types;
  }

  static void Register(RuntimeTypeInfo info) {
    GetTypes().insert(std::make_pair(info.id, info));
  }
};