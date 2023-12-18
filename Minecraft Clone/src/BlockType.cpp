#include "BlockType.h"

BlockType::BlockType()
{
	SideTexture = nullptr; TopTexture = nullptr;
	TopColor = color(); SideColor = color(); BottomColor = color();
	usesSameColor = true; usesSameTexture = true;
}

BlockType::BlockType(Texture* texture)
{
	SideTexture = texture; TopTexture = texture;
	usesSameTexture = true;

	TopColor = color(); SideColor = color(); BottomColor = color();
	usesSameColor = true;
}

BlockType::BlockType(Texture* sideTexture, Texture* topTexture)
{
	SideTexture = sideTexture;
	TopTexture = topTexture;
	if (sideTexture == topTexture)
		usesSameTexture = true;
	
	TopColor = color(); SideColor = color(); BottomColor = color();
	usesSameColor = true;
}

BlockType::BlockType(Texture* sideTexture, Texture* topTexture, color topColor, color sideColor, color bottomColor)
{
	SideTexture = sideTexture;
	TopTexture = topTexture;
	if (sideTexture == topTexture)
		usesSameTexture = true;

	TopColor = topColor; SideColor = sideColor; BottomColor = bottomColor;
	usesSameColor = (topColor == sideColor && sideColor == bottomColor);
}

BlockType::~BlockType()
{
}
