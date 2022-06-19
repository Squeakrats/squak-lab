#pragma once
#include "Actor.h"
#include "window.h"
#include <functional>
#include <memory>
#include <squak/core/AssetManager.h>
#include <squak/engine/RuntimeTypeInfo.h>
#include <squak/engine/rpc.h>
#include <squak/graphics/IRenderer.h>
#include <squak/graphics/SceneNode.h>
#include <string>

struct Axis {
  std::map<char, float> bindings;
};

class Engine {
private:
  std::shared_ptr<window::IWindow> window{};
  AssetManager assetManager{};
  std::shared_ptr<SceneNode> scene{};
  std::shared_ptr<CameraNode> camera{};
  std::shared_ptr<IRenderer> renderer{};
  std::pair<uint32_t, uint32_t> size{};

  std::map<std::string, std::shared_ptr<Actor>> actors{};
  uint64_t nextActorId{}; // TODO - use rng
  std::map<std::string, Axis> axes{};

  std::function<void(float deltaMs)> tick = [](float) {};

  Engine() : scene(std::make_shared<SceneNode>()){};
  std::shared_ptr<Actor> SpawnCore(RuntimeTypeInfo& info, Transform transform);

public:
  static Engine engine;
  static Engine& Init(std::string assetDir);
  static void Teardown();

  AssetManager& GetAssetManager() { return this->assetManager; };
  std::shared_ptr<SceneNode> GetScene() { return this->scene; };
  void OnTick(std::function<void(float deltaMs)> tick) { this->tick = tick; };

  std::shared_ptr<CameraNode> GetCamera() { return this->camera; };
  void SetCamera(std::shared_ptr<CameraNode> camera) { this->camera = camera; };

  std::shared_ptr<IRenderer> GetRenderer() { return this->renderer; };
  void SetRenderer(std::shared_ptr<IRenderer> renderer) {
    this->renderer = renderer;
  };

  std::pair<uint32_t, uint32_t> GetSize() { return this->size; }

  void Tick();
  void Run();
  bool isRunning() { return !window->ShouldClose(); }

  template<typename T,
           std::enable_if<std::is_base_of<Actor, T>::value>* = nullptr>
  std::shared_ptr<T> Spawn(Transform transform = Transform()) {
    return std::dynamic_pointer_cast<T>(
      SpawnCore(T::GetRuntimeTypeInfoInstance(), transform));
  }

  template<typename T>
  std::shared_ptr<T> GetAsset(std::string name) {
    return this->assetManager.Get<T>(name);
  }

  void AddChild(std::shared_ptr<Actor> child) {
    this->scene->AddChild(child->GetRoot());
  }

  int GetKey(int key) { return this->window->GetKey(key); }

  void RegisterAxis(std::string name, Axis axis) {
    this->axes.insert(std::make_pair(name, axis));
  }

  float GetAxis(std::string name) {
    Axis& axis = this->axes.at(name);
    for (auto binding : axis.bindings) {
      if (this->window->GetKey(binding.first)) {
        return binding.second;
      }
    }

    return 0.0f;
  }

  std::shared_ptr<Actor> Find(std::string id) {
    auto find = this->actors.find(id);
    return (find != this->actors.end()) ? find->second : nullptr;
  }

  void Invoke(RPC rpc) { this->Find(rpc.id)->Invoke(rpc); }

  static Engine Create(uint32_t width,
                       uint32_t height,
                       std::string name,
                       std::string assetDir);
};