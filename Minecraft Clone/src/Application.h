#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Texture.h"

class Application
{
public:
	Application();
	~Application();

	void mainloop();
private:
	GLFWwindow* window;

	int windowWidth = 480; int windowHeight = 480;

	float vertices[32] = {
		 //Positions		  //Colours			  // Texture
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	unsigned int indices[6] = { 
		0, 1, 3,
		1, 2, 3 
	};

	Shader shader;
	Texture texture;

	unsigned int VBO; // vertx buffer object
	unsigned int VAO; // vertex array object
	unsigned int EBO;

	void initTriangle();
	void draw();
	void terminate();
};

