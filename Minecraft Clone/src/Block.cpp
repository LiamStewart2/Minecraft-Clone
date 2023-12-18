#include "Block.h"

Block::Block(BlockType* blocksType, glm::vec3 position)
{
	info = blocksType;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
}

Block::~Block()
{
}

void Block::draw(Shader* shader)
{
	shader->setMat4("model", model);
	if (info->usesSameTexture)
	{
		info->SideTexture->useTexture();
		shader->setVec3("color", info->SideColor.r, info->SideColor.g, info->SideColor.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else 
	{
		info->SideTexture->useTexture();
		shader->setVec3("color", info->SideColor.r, info->SideColor.g, info->SideColor.b);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		info->TopTexture->useTexture();

		//Bottom Face
		if(info->usesSameColor != true)
			shader->setVec3("color", info->BottomColor.r, info->BottomColor.g, info->BottomColor.b);
		glDrawArrays(GL_TRIANGLES, 24, 6);

		//Top Face
		if(info->usesSameColor != true)
			shader->setVec3("color", info->TopColor.r, info->TopColor.g, info->TopColor.b);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}
}
