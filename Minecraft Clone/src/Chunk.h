#pragma once

#include "Block.h"
#include "Shader.h"
#include "BlockDatabase.h"
#include "PerlinNoise.h"

#include <algorithm>

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 32
#define CHUNK_OFFSET 126

class Chunk
{
public:
	Chunk() {}
	~Chunk() {  }
	Chunk(int x_pos, int y_pos, BlockDatabase* bdata, PerlinNoise* terrainHeight, PerlinNoise* treeMap);
	void drawChunk(Shader* shader);
	void updateEdgeCases(Chunk* otherChunk);

	glm::vec2 chunk_pos;

	Block* getBlock(int x, int y, int z);
private:
	Block* chunkMap[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT];
	BlockDatabase* bdata;

	void initChunk(int chunk_x, int chunk_y, PerlinNoise* terrainHeight);
	void addTrees(int chunk_x, int chunk_y, PerlinNoise* treeMap);

	// The position entered is the block just below the first block of the tree
	void placeTree(int x, int y, int z, int chunk_x, int chunk_y);

	void updateFaces();

	int get1DIndex(int x, int y, int z) {return x + CHUNK_WIDTH * (z + CHUNK_DEPTH * y);
	}
};

