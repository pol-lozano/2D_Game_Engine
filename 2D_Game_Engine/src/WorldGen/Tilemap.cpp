#include "Tilemap.h"
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <bitset>

void Tilemap::generate(int layer)
{
	randomFill(layer);
	for (int i = 0; i < 5; i++) {
		smoothMap(layer);
	}
}

void Tilemap::randomFill(int layer)
{
	//Initialize seed with hashcode or random seed
	randomSeed ? srand(time(NULL)) : srand(std::hash<std::string>{}(seed));

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//Place walls on tilemaps borders
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				setTile(x, y, layer, 1, 1);
			}
			else {
				auto value = (rand() % 100 < randomFillPercent) ? 1 : 0;
				setTile(x, y, layer, value, value);
			}
		}
	}
}

void Tilemap::smoothMap(int layer)
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int neighbourWallTiles = getSurroundingWallCount(x, y, layer);

			if (neighbourWallTiles > 4)
				setTile(x, y, layer, 1, 1);
			else if (neighbourWallTiles < 4)
				setTile(x, y, layer, 0, 0);
		}
	}
}

int Tilemap::getSurroundingWallCount(int x, int y, int layer)
{
	int wallCount = 0;
	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height) {
				if (neighbourX != x || neighbourY != y) {
					wallCount += getTileValue(neighbourX,neighbourY,layer);
				}
			}
			else {
				wallCount++;
			}
		}
	}
	return wallCount;
}

void Tilemap::autoTileAll()
{
	//Autotile each tile in each layer
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int i = 0; i < layers.size(); i++) {
				if (layers[i].isAutoTiled() && getTileValue(x, y, i)) 
				{
					autoTilePoint(x, y, i);
				}
			}
		} 
	}

}

void Tilemap::autoTileNeighbours(int x, int y, int layer) {
		//Top tiles
		autoTilePoint(x - 1, y + 1, layer);
		autoTilePoint(x, y + 1, layer);
		autoTilePoint(x + 1, y + 1, layer);
		//Center tiles
		autoTilePoint(x - 1, y, layer);
		autoTilePoint(x, y, layer);
		autoTilePoint(x + 1, y, layer);
		//Bottom tiles
		autoTilePoint(x - 1, y - 1, layer);
		autoTilePoint(x, y - 1, layer);
		autoTilePoint(x + 1, y - 1, layer);
}


void Tilemap::autoTilePoint(int x, int y, int layer)
{
	//Ignore empty tiles or out of bounds
	if(outOfBounds(x,y) || !getTileValue(x, y, layer)) return;

	std::bitset<4> bitmask;

	//Flipped upside down because of sdl coordinate system
	if(!outOfBounds(x, y + 1))
	bitmask[0] = (getTileValue(x, y + 1, layer) || y > height - 1);	//Top
	if (!outOfBounds(x - 1, y))
	bitmask[2] = (getTileValue(x - 1, y, layer) || x <= 0);	//Left
	if (!outOfBounds(x + 1, y))
	bitmask[1] = (getTileValue(x + 1, y, layer) || x >= width - 1); //Right
	if (!outOfBounds(x, y - 1))
	bitmask[3] = (getTileValue(x, y - 1, layer) || y <= 0);	//Bottom

	//std::cout << bitmask.to_ulong() << std::endl;
	setTile(x, y, layer, getTileValue(x, y, layer), static_cast<uint8_t>(bitmask.to_ulong()));
}
