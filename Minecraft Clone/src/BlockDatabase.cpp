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
	tDirt = Texture("res/images/blocks/dirt.png");
	tCobblestone = Texture("res/images/blocks/cobblestone.png");
	tOakLog = Texture("res/images/blocks/oak_log.png");
	tCherryLeaves = Texture("res/images/blocks/cherry_leaves.png");

	//Air
	bAir = BlockType();
	bAir.isAir = true;


	bGrass = BlockType(&tGrassSide, &tGrassTop, color(69, 255, 81), color(), color(156, 63, 37));
	bGrass.canSpawnTree = true;

	bDirt = BlockType(&tDirt);
	bCobblestone = BlockType(&tCobblestone);
	bOakLog = BlockType(&tOakLog);
	bCherryLeaves = BlockType(&tCherryLeaves);
}
