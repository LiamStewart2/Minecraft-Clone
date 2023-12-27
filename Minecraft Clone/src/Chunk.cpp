#include "Chunk.h"

Chunk::Chunk()
{

}

Chunk::Chunk(int x_pos, int y_pos, BlockDatabase* BlockData)
{
	bdata = BlockData;
	initChunk(x_pos, y_pos);
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
	double PI = 3.14159265;
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			if(chunk_x == 1 && chunk_y == 2)
			{
				for (int y = 0; y < CHUNK_HEIGHT; y++) {

					int index = get1DIndex(x, y, z);

					chunkMap[index] = new Block(&bdata->bAir, glm::vec3(x + (chunk_x * CHUNK_WIDTH), y, z + (chunk_y * CHUNK_DEPTH)));
				}
			}
			else {
				//int GrassHeight = CHUNK_HEIGHT / 2;
				int GrassHeight = (CHUNK_HEIGHT / 2) +
					(sin((((x + (chunk_x * CHUNK_WIDTH))) * CHUNK_WIDTH) * PI / 180)) * 4;
				if (x == 0 && z == 1)
				{
					GrassHeight = 2;
				}
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

void Chunk::updateEdgeCases(Chunk* otherChunk, Face Side)
{
	switch (Side) {
	case LEFT_SIDE:
		for(int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				chunkMap[get1DIndex(0, y, z)]->updateFace(LEFT_SIDE, otherChunk->getBlock(CHUNK_WIDTH - 1, y, z)->info->isAir);
			}
		}
	case RIGHT_SIDE:
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				chunkMap[get1DIndex(x, y, CHUNK_DEPTH - 1)]->updateFace(BACK_SIDE, otherChunk->getBlock(x, y, 0)->info->isAir);
		}
	case BACK_SIDE:
		for (int x = 0; x < CHUNK_DEPTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				chunkMap[get1DIndex(x, y, CHUNK_DEPTH - 1)]->updateFace(BACK_SIDE, otherChunk->getBlock(x, y, 0)->info->isAir);
		}
	case FRONT_SIDE:
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
				chunkMap[get1DIndex(x, y, 0)]->updateFace(FRONT_SIDE, otherChunk->getBlock(x, y, CHUNK_DEPTH - 1)->info->isAir);
		}
	}
}