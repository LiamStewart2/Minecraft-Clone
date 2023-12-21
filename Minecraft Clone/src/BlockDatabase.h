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
    BlockType bCobblestone;
    BlockType bAir;


private:
    Texture tGrassSide;
    Texture tGrassTop;
    Texture tCobblestone;
};

