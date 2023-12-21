#pragma once

#include "Texture.h"
#include "Color.h"

class BlockType
{
public:
	BlockType();
	BlockType(Texture* texture);
	BlockType(Texture* sideTexture, Texture* topTexture);
	BlockType(Texture* sideTexture, Texture* topTexture, color topColor, color sideColor, color bottomColor);
	~BlockType();

	Texture* SideTexture;
	Texture* TopTexture;

	color TopColor;
	color SideColor;
	color BottomColor;

	bool usesSameColor = false;
	bool usesSameTexture = false;
	bool isAir = false;
};

