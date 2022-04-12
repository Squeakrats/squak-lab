#include <memory>
#include <iostream>
#include <assert.h>
#include "gl/Renderer.h"
#include <GLFW/glfw3.h>
#include "SceneNode.h"

GLFWwindow* window = nullptr;
std::unique_ptr<gl::Renderer> renderer{};
std::unique_ptr<SceneNode> scene{};
std::unique_ptr<Matrix4> camera{};

void createWindow(int width, int height) {
    assert(glfwInit());
    window = glfwCreateWindow(width, height, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
}

uint32_t frameId = 0;

void tick() {
    scene->transform.SetPosition(std::sin(static_cast<float>(frameId) / 9000) * 300.0f, 0.0f, 0.f);

    renderer->Render(*camera, *scene);

    glfwSwapBuffers(window);
    glfwPollEvents();

    frameId++;
}

void start() {
#ifndef EMSCRIPTEN
    while (!glfwWindowShouldClose(window)) {
        tick();
    }
#else
    tick();
#endif
}

int main(int argc, char* argv[]) {
    createWindow(600, 600);
    renderer = std::make_unique<gl::Renderer>();
    camera = std::make_unique<Matrix4>(Matrix4::Orthographic(-300, 300, -300, 300, -300, 300));

    scene = std::make_unique<SceneNode>(Matrix4::Identity());

    std::vector<float> positions = {
         100.0,  100.0,
        -100.0,  100.0,
        -100.0, -100.0,

         100.0,  100.0,
        -100.0, -100.0,
         100.0, -100.0,
    };
    scene->geometry = std::make_unique<Geometry>(std::move(positions));

    start();

    return 0;
}