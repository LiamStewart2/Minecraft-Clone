#include "BlockDatabase.h"

BlockDatabase::BlockDatabase()
{
}

BlockDatabase::~BlockDatabase()
{
}

void BlockDatabase::initDatabase()
{
	//Textures
	tGrassSide = Texture("res/images/blocks/grass_block_side.png");
	tGrassTop = Texture("res/images/blocks/grass_block_top.png");
	tCobblestone = Texture("res/images/blocks/cobblestone.png");

	//Air
	bAir = BlockType();
	bAir.isAir = true;
	//Grass
	bGrass = BlockType(&tGrassSide, &tGrassTop, color(69, 255, 81), color(), color(156, 63, 37));
	//Cobblestone
	bCobblestone = BlockType(&tCobblestone);
}
