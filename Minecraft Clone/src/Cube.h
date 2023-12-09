#pragma once
#include "Vector3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Cube
{
public:
	Cube(Vector3 worldPosition);
	~Cube();

	void render();
private:
	Vector3 position;
	Vector3 scale;

	float vertices[36] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
	};

	unsigned int VBO;
};

