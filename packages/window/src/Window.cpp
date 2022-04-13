#include "Window.h"
#include <assert.h>

Window Window::Create(int width, int height, const char* name) {
    assert(glfwInit());
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
    glewExperimental = true;
    assert(glewInit() == GLEW_OK);
#endif

    return Window{ window };
}

void Window::Tick(std::function<void(void)> tick) {
#ifndef EMSCRIPTEN
    while (!glfwWindowShouldClose(this->window)) {
        tick();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
#else
    tick();
#endif
}