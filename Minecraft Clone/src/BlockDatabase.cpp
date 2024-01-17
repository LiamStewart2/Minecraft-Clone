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
	tOakLog = Texture("res/images/blocks/oak_log.png");

	//Air
	bAir = BlockType();
	bAir.isAir = true;


	bGrass = BlockType(&tGrassSide, &tGrassTop, color(69, 255, 81), color(), color(156, 63, 37));
	bCobblestone = BlockType(&tCobblestone);
	bOakLog = BlockType(&tOakLog);
}
