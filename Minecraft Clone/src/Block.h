#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "BlockType.h"
#include "Texture.h"
#include "Color.h"
#include "Shader.h"

class Block
{
public:
	Block() { info = nullptr; }
	Block(BlockType* blocksType, glm::vec3 position);
	~Block();

	void draw(Shader* shader);

	BlockType* info;

	glm::mat4 model;
};