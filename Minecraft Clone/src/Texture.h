#pragma once

#include <GL/glew.h>

#include <iostream>

class Texture
{
public:
	Texture(const char* ImagePath);
	Texture();
	~Texture();

	void useTexture();

	unsigned int textureID;

	int width, height, nrChannels;
};

