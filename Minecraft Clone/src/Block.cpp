#include "Block.h"

Block::Block(Texture* sideTexture, Texture* topTexture, color topColor, color sideColor, color bottomColor, glm::vec3 position)
{
	TopTexture = topTexture;
	SideTexture = sideTexture;

	if (topTexture == nullptr)
	{
		usesSameTexture = true;
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	TopColor = topColor; SideColor = sideColor; BottomColor = bottomColor;
}

Block::~Block()
{
}

void Block::draw(Shader* shader)
{
	shader->setMat4("model", model);
	if (usesSameTexture)
	{
		SideTexture->useTexture();
		shader->setVec3("color", SideColor.r, SideColor.g, SideColor.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else 
	{
		SideTexture->useTexture();
		shader->setVec3("color", SideColor.r, SideColor.g, SideColor.b);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		TopTexture->useTexture();

		//Bottom Face
		shader->setVec3("color", BottomColor.r, BottomColor.g, BottomColor.b);
		glDrawArrays(GL_TRIANGLES, 24, 6);

		//Top Face
		shader->setVec3("color", TopColor.r, TopColor.g, TopColor.b);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}
}
