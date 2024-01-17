#pragma once

#include "Block.h"
#include "Shader.h"
#include "BlockDatabase.h"
#include "PerlinNoise.h"

#include <algorithm>

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 32
#define CHUNK_OFFSET 50

#include <random>

class Chunk
{
public:
	Chunk() {}
	~Chunk() {  }
	Chunk(int x_pos, int y_pos, BlockDatabase* bdata, PerlinNoise* terrainHeight);
	void drawChunk(Shader* shader);
	void updateEdgeCases(Chunk* otherChunk);

	glm::vec2 chunk_pos;

	Block* getBlock(int x, int y, int z);
private:
	Block* chunkMap[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT];
	BlockDatabase* bdata;

	void initChunk(int chunk_x, int chunk_y, PerlinNoise* terrainHeight);
	void addTrees(int chunk_x, int chunk_y);

	// The position entered is the block just below the first block of the tree
	void placeTree(int x, int y, int z, int chunk_x, int chunk_y);

	void updateFaces();

	int get1DIndex(int x, int y, int z) { return x + CHUNK_WIDTH * (z + CHUNK_DEPTH * y); }

	int getRandomNumber(int min, int max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(gen);
	}
};

