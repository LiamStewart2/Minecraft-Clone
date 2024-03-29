#pragma once

#include "BlockType.h"
#include "Texture.h"

class BlockDatabase
{
public:
    BlockDatabase();
    ~BlockDatabase();

    void initDatabase();

    BlockType bGrass;
    BlockType bDirt;
    BlockType bCobblestone;
    BlockType bAir;
    BlockType bOakLog;
    BlockType bCherryLeaves;

private:
    Texture tGrassSide;
    Texture tGrassTop;
    Texture tDirt;
    Texture tCobblestone;
    Texture tOakLog;
    Texture tCherryLeaves;
};

