#include "utility.h"
#include <chrono>
#include <squak/engine/Engine.h>
#include <squak/engine/JSONLoader.h>
#include <squak/net.h>
#ifndef EMSCRIPTEN
#include <squak/net/Poller.h>
#endif
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

Engine Engine::engine = Engine{};

Engine& Engine::Init(std::string assetDir) {
  net::Init();
  InitLogger(assetDir + "/log.txt");

  engine.assetManager.SetBasePath(assetDir);
  engine.assetManager.Register(std::make_shared<JSONLoader>());

  auto& config = engine.assetManager.Get<JSONAsset>("settings.json")->json;
  auto& window = config["window"];
  std::string name = window["name"].get<std::string>();
  json::Array& size = window["size"].get<json::Array>();

  engine.size.first = size[0].as<uint32_t>();
  engine.size.second = size[1].as<uint32_t>();

  engine.window =
    window::Create("Lawless", engine.size.first, engine.size.second);

  for (auto& entry : config["axes"].get<json::Object>().entries) {
    Axis axis{};
    for (auto& binding : entry.second.get<json::Object>().entries) {
      axis.bindings.insert(
        std::make_pair(binding.first[0], binding.second.as<float>()));
    }

    engine.RegisterAxis(entry.first, axis);
  }

  return engine;
}

void Engine::Teardown() {
  net::Teardown();
}

#ifndef EMSCRIPTEN
void Engine::Run() {
  while (engine.isRunning()) {
    engine.Tick();
  }
}
#else
void Engine_loop() {
  Engine::engine.Tick();
}

void Engine::Run() {
  emscripten_set_main_loop(Engine_loop, 0, true);
}
#endif

void Engine::Tick() {
  window->Poll();
#ifndef EMSCRIPTEN
  net::Poller::Instance().Poll();
#endif

  static auto lastTick = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  float deltaMs = static_cast<float>(
    std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick)
      .count());

  if (deltaMs < 17) {
    return;
  }

  lastTick = now;

  for (auto actor : this->actors) {
    actor.second->Tick(deltaMs);
  }

  this->tick(deltaMs);

  if (this->renderer != nullptr) {
    this->renderer->Render(*this->camera, *this->scene);
  }
}

std::shared_ptr<Actor> Engine::SpawnCore(RuntimeTypeInfo& info,
                                         Transform transform) {
  ActorInitializer initializer{ std::to_string(this->nextActorId++),
                                transform,
                                *this };
  std::shared_ptr<Actor> object = info.create(initializer);
  this->actors.insert(std::make_pair(initializer.id, object));
  this->AddChild(object);

  return object;
}