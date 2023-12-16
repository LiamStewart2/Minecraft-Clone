#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* ImagePath)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(ImagePath, &width, &height, &nrChannels, 0);
	std::cout << nrChannels << std::endl;

	if (data)
	{
		GLint channel = -1;
		if (nrChannels == 3)
			channel = GL_RGB;
		else
			channel = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "TEXTURE::LOAD FAILED" << std::endl;
	stbi_image_free(data);
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::useTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}
