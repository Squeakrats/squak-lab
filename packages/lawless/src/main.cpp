#include <assert.h>
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include "SceneNode.h"

GLFWwindow* createWindow() {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(600, 600, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
#endif

    return window;
}

void tick(GLFWwindow* window) {
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void start(GLFWwindow* window) {
#ifndef EMSCRIPTEN
    while (!glfwWindowShouldClose(window)) {
        tick(window);
    }
#else
    tick();
#endif
}

int main(int argc, char* argv[]) {
    GLFWwindow* window = createWindow();

    SceneNode scene();

    start(window);

    return 0;
}