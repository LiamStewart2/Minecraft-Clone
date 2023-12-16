#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Texture.h"
#include "Color.h"
#include "Shader.h"

class Block
{
public:
	Block() { TopTexture = nullptr; SideTexture = nullptr; }
	Block(Texture* sideTexture, Texture* topTexture, color topColor, color sideColor, color bottomColor, glm::vec3 position);
	~Block();



	void draw(Shader* shader);

	color TopColor;
	color SideColor;
	color BottomColor;

	Texture* TopTexture;
	Texture* SideTexture;

	glm::mat4 model;

	bool usesSameTexture = false;
};