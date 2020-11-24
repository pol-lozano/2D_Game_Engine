#include "Tilemap.h"
#include "Core/Core.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components/Components.h"

//Manager defined in core.cpp
extern EntityManager manager;

Tilemap::Tilemap(const char* mp, int ms, int ts) : mapPath(mp), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Tilemap::~Tilemap()
{
}

void Tilemap::loadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			addTile(srcX, srcY, x * scaledSize , y * scaledSize);
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				//tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Tilemap::addTile(int srcX, int srcY, int posX, int posY)
{
	auto& tile(manager.addEntity());
	//tile.addComponent<TileComponent>(srcX, srcY, posX, posY, tileSize, mapScale, mapPath);
}
