#include "Window.h"
#include <assert.h>
#ifndef EMSCRIPTEN
#include <gl/glew.h>
#endif
#include "GLFW/glfw3.h"

Window Window::Create(int width, int height, const char* name) {
	GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    assert(glfwInit());
    window = glfwCreateWindow(width, height, "Lawless", nullptr, nullptr);
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