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

void tick() {
    renderer->Render(*camera, *scene);

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
    createWindow(600, 600);
    scene = std::make_unique<SceneNode>(Matrix4::Identity());
    renderer = std::make_unique<gl::Renderer>();
    camera = std::make_unique<Matrix4>(Matrix4::Orthographic(-300, 300, -300, 300, -300, 300));

    start();

    return 0;
}