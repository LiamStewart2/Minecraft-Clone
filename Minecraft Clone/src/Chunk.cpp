#include "Chunk.h"

#define SEED 1015837287839

// CODE FOR PERLIN NOISE // FROM CHATGPT //
#include <iostream>
#include <cmath>
#include <algorithm>

class PerlinNoise {
public:
	PerlinNoise(int seed, double offsetX, double offsetY) {
		// Generate a random permutation of 0 to 255
		for (int i = 0; i < 256; ++i) {
			permutation[i] = i;
		}
		std::random_shuffle(permutation, permutation + 256);
		// Duplicate the permutation array to avoid overflow
		std::copy(permutation, permutation + 256, permutation + 256);

		applyOffset(offsetX, offsetY);
	}

	double noise(double x, double y) const {
		int X = static_cast<int>(std::floor(x)) & 255;
		int Y = static_cast<int>(std::floor(y)) & 255;
		x -= std::floor(x);
		y -= std::floor(y);
		double u = fade(x);
		double v = fade(y);

		int A = permutation[X] + Y;
		int AA = permutation[A];
		int AB = permutation[A + 1];

		int B = permutation[X + 1] + Y;
		int BA = permutation[B];
		int BB = permutation[B + 1];

		return lerp(v, lerp(u, grad(permutation[AA], x, y), grad(permutation[BA], x - 1, y)),
			lerp(u, grad(permutation[AB], x, y - 1), grad(permutation[BB], x - 1, y - 1)));
	}

private:
	void applyOffset(double offsetX, double offsetY) {
		for (int i = 0; i < 256; ++i) {
			permutation[i] = (permutation[i] + static_cast<int>(offsetX)) & 255;
			permutation[i + 256] = (permutation[i] + static_cast<int>(offsetY)) & 255;
		}
	}

	double fade(double t) const {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double lerp(double t, double a, double b) const {
		return a + t * (b - a);
	}

	double grad(int hash, double x, double y) const {
		int h = hash & 7; // Gradient value 0-7
		double grad = 1 + (h & 3); // Gradient value 1-4
		if (h & 4) grad = -grad; // Randomly invert half of the gradients
		return (grad * x + grad * y);
	}

	int permutation[512];
};
// END OF CHATGPT CTRL C CTRL V


Chunk::Chunk(int x_pos, int y_pos, BlockDatabase* BlockData)
{
	bdata = BlockData;
	initChunk(x_pos, y_pos);
	chunk_pos = glm::vec2(x_pos, y_pos);
}

void Chunk::drawChunk(Shader* shader)
{
	for (int i = 0; i < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT; i++)
	{
		if (chunkMap[i]->info->isAir == false)
			chunkMap[i]->draw(shader);
	}
	
}

void Chunk::initChunk(int chunk_x, int chunk_y)
{
	PerlinNoise perlin = PerlinNoise(SEED, chunk_x, chunk_y);
	double PI = 3.14159265;
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			int world_x = x + (chunk_x * CHUNK_WIDTH);
			int world_y = z + (chunk_y * CHUNK_DEPTH);

			//int GrassHeight = CHUNK_HEIGHT / 2;
			/*int GrassHeight = (int)floor((CHUNK_HEIGHT / 2) +

				(sin((((x + (chunk_x * CHUNK_WIDTH))) * CHUNK_WIDTH) * PI / 180)) *
				(sin((((z + (chunk_y * CHUNK_WIDTH))) * CHUNK_WIDTH) * PI / 180))
				* 10);*/
			double perlinvalue = perlin.noise(static_cast<double>(world_x) / CHUNK_WIDTH, static_cast<double>(world_y) / CHUNK_WIDTH);
			int GrassHeight = (CHUNK_HEIGHT / 2) + int(perlinvalue);
			for (int y = 0; y < GrassHeight; y++)
			{
				int index = get1DIndex(x, y, z);

				chunkMap[index] = new Block(&bdata->bCobblestone, glm::vec3(x + (chunk_x * CHUNK_WIDTH), y, z + (chunk_y * CHUNK_DEPTH)));
			}
			chunkMap[get1DIndex(x, GrassHeight, z)] = new Block(&bdata->bGrass, glm::vec3(x + (chunk_x * CHUNK_WIDTH), GrassHeight, z + (chunk_y * CHUNK_DEPTH)));
			for (int y = GrassHeight + 1; y < CHUNK_HEIGHT; y++)
			{
				int index = get1DIndex(x, y, z);

				chunkMap[index] = new Block(&bdata->bAir, glm::vec3(x + (chunk_x * CHUNK_WIDTH), y, z + (chunk_y * CHUNK_DEPTH)));
			}
		}
	}

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				Block* ThisBlock = chunkMap[get1DIndex(x, y, z)];
				if (ThisBlock->info->isAir == false)
				{
					//check Bottom
					if (y > 0)
						ThisBlock->updateFace(BOTTOM_SIDE, chunkMap[get1DIndex(x, y - 1, z)]->info->isAir);
					//check Top
					if (y < CHUNK_HEIGHT - 1)
						ThisBlock->updateFace(TOP_SIDE, chunkMap[get1DIndex(x, y + 1, z)]->info->isAir);

					//check Left
					if (x > 0)
						ThisBlock->updateFace(LEFT_SIDE, chunkMap[get1DIndex(x - 1, y, z)]->info->isAir);
					//check Right
					if (x < CHUNK_WIDTH - 1)
						ThisBlock->updateFace(RIGHT_SIDE, chunkMap[get1DIndex(x + 1, y, z)]->info->isAir);

					//check Front
					if (z > 0)
					{
						ThisBlock->updateFace(FRONT_SIDE, chunkMap[get1DIndex(x, y, z - 1)]->info->isAir);
					}
					//check Back
					if (z < CHUNK_DEPTH - 1)
						ThisBlock->updateFace(BACK_SIDE, chunkMap[get1DIndex(x, y, z + 1)]->info->isAir);
				}
			}
		}
	}
}

Block* Chunk::getBlock(int x, int y, int z)
{
	return chunkMap[get1DIndex(x, y, z)];
}

void Chunk::updateEdgeCases(Chunk* otherChunk)
{
	//CHECK Z AXIS // INCREASE IN X AXIS
	if (getBlock(0, 0, 0)->position.x < otherChunk->getBlock(0, 0, 0)->position.x)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				getBlock(CHUNK_WIDTH - 1, y, z)->updateFace(RIGHT_SIDE, otherChunk->getBlock(0, y, z)->info->isAir);
		}
	}

	//CHECK Z AXIS // DECREASE IN X AXIS
	if (getBlock(0, 0, 0)->position.x > otherChunk->getBlock(0, 0, 0)->position.x)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				getBlock(0, y, z)->updateFace(LEFT_SIDE, otherChunk->getBlock(CHUNK_WIDTH - 1, y, z)->info->isAir);
		}
	}


	//CHECK X AXIS // INCREASE IN Z AXIS
	else if (getBlock(0, 0, 0)->position.z < otherChunk->getBlock(0, 0, 0)->position.z)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				getBlock(x, y, CHUNK_DEPTH - 1)->updateFace(BACK_SIDE, otherChunk->getBlock(x, y, 0)->info->isAir);
		}
	}
	//CHECK X AXIS // DECREASE IN Z AXIS
	else if (getBlock(0, 0, 0)->position.z > otherChunk->getBlock(0, 0, 0)->position.z)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				getBlock(x, y, 0)->updateFace(FRONT_SIDE, otherChunk->getBlock(x, y, CHUNK_DEPTH - 1)->info->isAir);
		}
	}
}