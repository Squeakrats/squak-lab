#include "Engine.h"
#include "utility.h"
#include <chrono>
#include <filesystem>

Engine Engine::Create(uint32_t width, uint32_t height, std::string name, std::string assetDir) {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    Log(std::format("Working Directory : {}", std::filesystem::current_path().string()));
    Log(std::format("OpenGL Version : {}", std::string((char*)glGetString(GL_VERSION))));

    return Engine(window, assetDir);
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

std::shared_ptr<Actor> Engine::SpawnCore(std::string type) {
    std::string id = "" + this->nextActorId++;
    std::shared_ptr<Actor> object = this->creators.at(type)(id);
    this->actors.insert(std::make_pair(id, object));

    return object;
}