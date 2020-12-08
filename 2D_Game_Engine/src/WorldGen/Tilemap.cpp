#include "Tilemap.h"
#include <stdlib.h>
#include <time.h> 

void Tilemap::generate()
{
	randomFill();
	for (int i = 0; i < 5; i++) {
		smoothMap();
	}
}

void Tilemap::randomFill()
{
	//Initialize seed with hashcode or random seed
	randomSeed ? srand(time(NULL)) : srand(std::hash<std::string>{}(seed));

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//Place walls on tilemaps borders
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				this->operator()(x, y) = 1;
			}
			else {
				this->operator()(x, y) = (rand() % 100 < randomFillPercent) ? 1 : 0;
			}
		}
	}
}

void Tilemap::smoothMap()
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int neighbourWallTiles = getSurroundingWallCount(x, y);

			if (neighbourWallTiles > 4)
				this->operator()(x, y) = 1;
			else if (neighbourWallTiles < 4)
				this->operator()(x, y) = 0;
		}
	}
}

int Tilemap::getSurroundingWallCount(int x, int y)
{
	int wallCount = 0;
	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height) {
				if (neighbourX != x || neighbourY != y) {
					wallCount += this->operator()(neighbourX, neighbourY);
				}
			}
			else {
				wallCount++;
			}
		}
	}
	return wallCount;
}
