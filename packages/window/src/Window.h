#pragma once
#include <functional>

struct GLFWwindow;

class Window {
private:
	GLFWwindow* window;

	Window(GLFWwindow* window) : window(window) {};

public:
	void Tick(std::function<void(void)> tick);

	static Window Create(int width, int height, const char* name);
};