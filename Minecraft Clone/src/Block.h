#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "BlockType.h"
#include "Texture.h"
#include "Color.h"
#include "Shader.h"

enum Face {
	TOP_SIDE, BOTTOM_SIDE, BACK_SIDE, FRONT_SIDE, LEFT_SIDE, RIGHT_SIDE
};

class Block
{
public:
	Block() { info = nullptr; model = glm::mat4(1.0f); }
	Block(BlockType* blocksType, glm::vec3 position);
	~Block();

	void updateFace(Face face, bool state);
	void draw(Shader* shader);

	BlockType* info;

	bool anyFaces = false;
	bool showFaces[6] = {false, false, false, false, false, false};

	glm::mat4 model;
private:
	void checkIfAnyFacesVisible();
};