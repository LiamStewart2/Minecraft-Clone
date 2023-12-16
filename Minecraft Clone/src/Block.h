#pragma once
#include "Texture.h"
#include "Color.h"
#include "Shader.h"

class Block
{
public:
	Block() {};
	Block(const char* top_texture, const char* side_texture, color topColor, color sideColor, color bottomColor);
	~Block();

	void draw(Shader* shader);

	color TopColor;
	color SideColor;
	color BottomColor;

	Texture TopTexture;
	Texture SideTexture;
};

