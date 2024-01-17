#include "Chunk.h"

Chunk::Chunk(int x_pos, int y_pos, BlockDatabase* BlockData, PerlinNoise* terrainHeight, PerlinNoise* treeMap)
{
	bdata = BlockData;
	initChunk(x_pos, y_pos, terrainHeight);
	addTrees(x_pos, y_pos, treeMap);
	updateFaces();
	chunk_pos = glm::vec2(x_pos, y_pos);
}

void Chunk::drawChunk(Shader* shader)
{
	for (int i = 0; i < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT; i++)
	{
		if (chunkMap[i]->info->isAir == false && chunkMap[i]->anyFaces)
			chunkMap[i]->draw(shader);
	}
}

void Chunk::initChunk(int chunk_x, int chunk_y, PerlinNoise* terrainHeight)
{
	int chunkStartX = (chunk_x + CHUNK_OFFSET) * CHUNK_WIDTH;
	int chunkStartY = (chunk_y + CHUNK_OFFSET) * CHUNK_DEPTH;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			double noiseValue = terrainHeight->octaveNoise((chunkStartX + x) / 32.0, (chunkStartY + z) / 32.0, 3, 0.3);
			int GrassHeight = (CHUNK_HEIGHT / 2) + static_cast<int>(CHUNK_HEIGHT * noiseValue);
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
}

void Chunk::updateFaces()
{
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

void Chunk::addTrees(int chunk_x, int chunk_y, PerlinNoise* treeMap)
{
	int chunkStartX = (chunk_x + CHUNK_OFFSET) * CHUNK_WIDTH;
	int chunkStartY = (chunk_y + CHUNK_OFFSET) * CHUNK_DEPTH;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			double noiseValue = treeMap->octaveNoise((chunkStartX + x) / 16.0, (chunkStartY + z) / 16.0, 2, 0.3);
			if (noiseValue == (double)0.0)
			{
				for (int y = CHUNK_HEIGHT - 1; y--; y < 1)
				{
					if (getBlock(x, y, z)->info->isAir == false)
					{
						placeTree(x, y, z, chunk_x, chunk_y);
					}
				}
			}
		}
	}
}
void Chunk::placeTree(int x, int y, int z, int chunk_x, int chunk_y)
{
	int index = get1DIndex(x, y + 1, z);

	delete(chunkMap[index]);
	chunkMap[index] = new Block(&bdata->bCobblestone, glm::vec3(x + (chunk_x * CHUNK_WIDTH), y + 1, z + (chunk_y * CHUNK_DEPTH)));
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