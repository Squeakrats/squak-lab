#pragma once

struct GLFWwindow;

class Window {
private:
	GLFWwindow* window;

	Window(GLFWwindow* window) : window(window) {};

public:
	bool ShouldClose();
	void Poll();
	void Swap();

	static Window Create(int width, int height, const char* name);
};