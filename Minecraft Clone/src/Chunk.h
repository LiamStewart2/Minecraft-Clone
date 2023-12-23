#pragma once

#include "Block.h"
#include "Shader.h"
#include "BlockDatabase.h"

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 32

class Chunk
{
public:
	Chunk();
	Chunk(int x_pos, int y_pos, BlockDatabase* bdata);
	void drawChunk(Shader* shader);
	void updateEdgeCases(Chunk* otherChunk, Face Side);

	Block* getBlock(int x, int y, int z);
private:
	Block* chunkMap[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT];
	BlockDatabase* bdata;

	void initChunk(int chunk_x, int chunk_y);
	int get1DIndex(int x, int y, int z) {return x + CHUNK_WIDTH * (z + CHUNK_DEPTH * y);
	}
};

