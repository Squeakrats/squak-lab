#pragma once
#include <functional>
#ifndef EMSCRIPTEN
#include <gl/glew.h>
#endif
#include "GLFW/glfw3.h"

class Window {
public:
	GLFWwindow* window;

	Window(GLFWwindow* window) : window(window) {};

public:
	void Tick(std::function<void(void)> tick);

	static Window Create(int width, int height, const char* name);
};