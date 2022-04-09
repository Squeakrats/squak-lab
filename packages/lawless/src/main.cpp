#include <memory>
#include <iostream>
#include <assert.h>
#include "gl/Renderer.h"
#include <GLFW/glfw3.h>
#include "SceneNode.h"


GLFWwindow* window = nullptr;
std::unique_ptr<gl::Renderer> renderer{};
std::unique_ptr<SceneNode> scene{};

void createWindow() {
    assert(glfwInit());
    window = glfwCreateWindow(600, 600, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
}

void tick() {
    renderer->Render(*scene);

    glfwSwapBuffers(window);
    glfwPollEvents();
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
    createWindow();
    scene = std::make_unique<SceneNode>();
    renderer = std::make_unique<gl::Renderer>();

    start();

    return 0;
}