#include "Engine.h"
#include <assert.h>
#include <chrono>

Engine Engine::Create(uint32_t width, uint32_t height, std::string name, std::string assetDir) {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

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

    this->tick(deltaMs);

    if (this->renderer != nullptr) {
        this->renderer->Render(*this->camera, *this->scene);
    }

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}