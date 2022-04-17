#include "Engine.h"
#include "utility.h"
#include <chrono>
#include <filesystem>


std::map<std::string, ActorCreator>& Engine::GetCreators() {
    static std::map<std::string, ActorCreator> creators{};

    return creators;
}

void Engine::InitWindow(uint32_t width, uint32_t height, std::string name) {
    assert(glfwInit());
    this->window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    Log(std::format("OpenGL Version : {}", std::string((char*)glGetString(GL_VERSION))));
}


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
    ActorInitializer initializer{ "" + this->nextActorId++, transform, *this };
    std::shared_ptr<Actor> object = GetCreators().at(type)(initializer);
    this->actors.insert(std::make_pair(initializer.id, object));

    return object;
}