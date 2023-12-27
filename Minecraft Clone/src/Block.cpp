#include "Block.h"

Block::Block(BlockType* blocksType, glm::vec3 position)
{
	info = blocksType;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
}

void Block::updateFace(Face face, bool state)
{
	showFaces[face] = state;
	if(state == true)
		checkIfAnyFacesVisible();
}

void Block::checkIfAnyFacesVisible()
{
	anyFaces = false;
	for (int i = 0; i < 6; i++)
	{
		if (showFaces[i] == true)
			anyFaces = true;
	}
}

Block::~Block()
{
}

void Block::draw(Shader* shader)
{
	if(anyFaces)
	{
		shader->setMat4("model", model);
		if (info->usesSameTexture)
		{
			info->SideTexture->useTexture();
			shader->setVec3("color", info->SideColor.r, info->SideColor.g, info->SideColor.b);

			if (showFaces[BACK_SIDE])
				glDrawArrays(GL_TRIANGLES, 6, 6);
			if (showFaces[FRONT_SIDE])
				glDrawArrays(GL_TRIANGLES, 0, 6);
			if (showFaces[LEFT_SIDE])
				glDrawArrays(GL_TRIANGLES, 12, 6);
			if (showFaces[RIGHT_SIDE])
				glDrawArrays(GL_TRIANGLES, 18, 6);
			if (showFaces[BOTTOM_SIDE])
				glDrawArrays(GL_TRIANGLES, 24, 6);
			if (showFaces[TOP_SIDE])
				glDrawArrays(GL_TRIANGLES, 30, 6);
		}
		else
		{

			info->SideTexture->useTexture();
			shader->setVec3("color", info->SideColor.r, info->SideColor.g, info->SideColor.b);

			if (showFaces[BACK_SIDE])
				glDrawArrays(GL_TRIANGLES, 6, 6);
			if (showFaces[FRONT_SIDE])
				glDrawArrays(GL_TRIANGLES, 0, 6);
			if (showFaces[LEFT_SIDE])
				glDrawArrays(GL_TRIANGLES, 12, 6);
			if (showFaces[RIGHT_SIDE])
				glDrawArrays(GL_TRIANGLES, 18, 6);

			if(showFaces[BOTTOM_SIDE] || showFaces[TOP_SIDE])
			{
				info->TopTexture->useTexture();

				//Bottom Face
				if (showFaces[BOTTOM_SIDE])
				{
					if (info->usesSameColor != true)
						shader->setVec3("color", info->BottomColor.r, info->BottomColor.g, info->BottomColor.b);
					glDrawArrays(GL_TRIANGLES, 24, 6);
				}

				//Top Face
				if (showFaces[TOP_SIDE])
				{
					if (info->usesSameColor != true)
						shader->setVec3("color", info->TopColor.r, info->TopColor.g, info->TopColor.b);
					glDrawArrays(GL_TRIANGLES, 30, 6);
				}
			}
		}
	}
}
