#include <iostream>
#include <assert.h>
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(600, 600, "Lawless", nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
#endif

    glClearColor(1, 0, 0, 1);
   
#ifndef EMSCRIPTEN
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#else
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
#endif

    return 0;
}