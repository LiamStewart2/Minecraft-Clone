#include "Block.h"

Block::Block(const char* top_texture, const char* side_texture, color topColor, color sideColor, color bottomColor)
{
	TopTexture = Texture(top_texture);
	SideTexture = Texture(top_texture);
	if (side_texture != "")
		SideTexture = Texture(side_texture);

	TopColor = topColor; SideColor = sideColor; BottomColor = bottomColor;
}

Block::~Block()
{
}

void Block::draw(Shader* shader)
{
	//Side Faces
	SideTexture.useTexture();
	shader->setVec3("color", SideColor.r, SideColor.g, SideColor.b);
	glDrawArrays(GL_TRIANGLES, 0, 24);

	TopTexture.useTexture();
	//Bottom Face
	shader->setVec3("color", BottomColor.r, BottomColor.g, BottomColor.b);
	glDrawArrays(GL_TRIANGLES, 24, 6);

	//Top Face
	shader->setVec3("color", TopColor.r, TopColor.g, TopColor.b);
	glDrawArrays(GL_TRIANGLES, 30, 6);

}
