#pragma once
#include <string>

class Tilemap
{
public:

	Tilemap(const char* mapPath, int mapScale, int tileSize);
	~Tilemap();

	void loadMap(std::string path,int sizeX,int sizeY);
	void addTile(int srcX, int srcY, int posX, int posY);
private:
	const char* mapPath;
	int mapScale;
	int tileSize;
	int scaledSize;
};

