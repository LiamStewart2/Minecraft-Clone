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

	int windowWidth = 480; int windowHeight = 480;

	float vertices[12] = {
	 0.5f,  0.5f, 0.0f,  
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f   
	};
	unsigned int indices[6] = { 
		0, 1, 3,
		1, 2, 3 
	};

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int shaderProgram;
	unsigned int VBO; // vertx buffer object
	unsigned int VAO; // vertex array object
	unsigned int EBO;

	void initTriangle();
	void draw();
	void terminate();
};

