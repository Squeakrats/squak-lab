#include <memory>
#include <assert.h>
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include "SceneNode.h"
#include "Renderer.h"

GLFWwindow* window = nullptr;
std::unique_ptr<SceneNode> scene{};

void createWindow() {
    assert(glfwInit());
    window = glfwCreateWindow(600, 600, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
#endif
}

void tick() {
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
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
    gl::Renderer renderer{};

    start();

    return 0;
}