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

bool Window::ShouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Window::Poll() {
    glfwPollEvents();
}

void Window::Swap() {
    glfwSwapBuffers(this->window);
}