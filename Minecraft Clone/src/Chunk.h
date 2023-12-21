#pragma once

#include "Block.h"
#include "Shader.h"
#include "BlockDatabase.h"

class Chunk
{
public:
	Chunk();
	Chunk(int x_pos, int y_pos, BlockDatabase* bdata);
	void drawChunk(Shader* shader);
private:
	// will be 16*16*128
	const int static CHUNK_WIDTH = 16; const int static CHUNK_DEPTH = 16;
	const int static CHUNK_HEIGHT = 32;

	Block chunkMap[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT];

	void initChunk(int chunk_x, int chunk_y, BlockDatabase* bdata);
	int get1DIndex(int x, int y, int z) {return x + CHUNK_WIDTH * (z + CHUNK_DEPTH * y);
	}
};

