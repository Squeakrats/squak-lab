#include <memory>
#include <iostream>
#include <assert.h>
#include "gl/Renderer.h"
#include "Window.h"
#include "SceneNode.h"

std::unique_ptr<Window> window = nullptr;
std::unique_ptr<gl::Renderer> renderer{};
std::unique_ptr<SceneNode> scene{};
std::unique_ptr<Matrix4> camera{};
uint32_t frameId = 0;

void tick() {
    scene->transform.SetPosition(std::sin(static_cast<float>(frameId) / 9000) * 300.0f, 0.0f, 0.f);
    renderer->Render(*camera, *scene);
    frameId++;
}

void start() {
#ifndef EMSCRIPTEN
    while (!window->ShouldClose()) {
        tick();
        window->Swap();
        window->Poll();
    }
#else
    tick();
#endif
}

int main(int argc, char* argv[]) {
    window = std::make_unique<Window>(Window::Create(600, 600, "Lawless"));
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    renderer = std::make_unique<gl::Renderer>();
    camera = std::make_unique<Matrix4>(Matrix4::Orthographic(-300, 300, -300, 300, -300, 300));

    scene = std::make_unique<SceneNode>(Matrix4::Identity());
    scene->geometry = std::make_unique<Geometry>(Geometry::CreatePlane(200, 200));

    start();

    return 0;
}