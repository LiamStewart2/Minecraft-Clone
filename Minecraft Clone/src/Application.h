#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Application
{
public:
	Application();
	~Application();

	void mainloop();
private:
	GLFWwindow* window;

	int windowWidth = 1080; int windowHeight = 720;

	void terminate();
};

