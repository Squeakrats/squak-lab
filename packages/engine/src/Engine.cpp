#include "Engine.h"
#include "utility.h"
#include <chrono>
#include "JSONLoader.h"

Engine Engine::engine = Engine{};

std::map<std::string, ActorCreator>& Engine::GetCreators() {
    static std::map<std::string, ActorCreator> creators{};

    return creators;
}

Engine& Engine::Init(std::string assetDir) {
    engine.assetManager.SetBasePath(assetDir);
    engine.assetManager.Register(std::make_shared<JSONLoader>());

    auto& config = engine.assetManager.Get<JSONAsset>("settings.json")->json;
    auto& window = config["window"];
    std::string name = window["name"].get<std::string>();
    json::Array& size = window["size"].get<json::Array>();

    for (auto& entry : config["axes"].get<json::Object>().entries) {
        Axis axis{};
        for (auto& binding : entry.second.get<json::Object>().entries) {
            axis.bindings.insert(std::make_pair(binding.first[0], static_cast<float>(binding.second.get<double>())));
        }

        engine.RegisterAxis(entry.first, axis);
    }

    assert(glfwInit());
    engine.window = glfwCreateWindow(static_cast<uint32_t>(size[0].get<double>()), static_cast<uint32_t>(size[1].get<double>()), name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(engine.window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    Log(std::format("OpenGL Version : {}", std::string((char*)glGetString(GL_VERSION))));

    return engine;
}

#ifndef EMSCRIPTEN
void Engine::Run() {
    while (engine.isRunning()) { engine.Tick(); }
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
    static auto lastTick = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();
    float deltaMs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count());
    
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

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

std::shared_ptr<Actor> Engine::SpawnCore(std::string type, Transform transform) {
    ActorInitializer initializer{ std::to_string(this->nextActorId++), transform, *this };
    std::shared_ptr<Actor> object = GetCreators().at(type)(initializer);
    this->actors.insert(std::make_pair(initializer.id, object));
    this->AddChild(object);

    return object;
}