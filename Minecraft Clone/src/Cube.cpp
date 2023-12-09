#include "Cube.h"

Cube::Cube(Vector3 worldPosition)
{
	glGenBuffers(1, &VBO);
}

Cube::~Cube()
{
}

void Cube::render()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}
