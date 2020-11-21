#pragma once
#include <SDL.h>
#include "Sprite.h"

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//Tile definitions
enum TileType
{
	TILE_RED = 0,
	TILE_GREEN = 1,
	TILE_BLUE = 2, 
	TILE_CENTER = 3,
	TILE_TOP = 4,
	TILE_TOPRIGHT = 5,
	TILE_RIGHT = 6,
	TILE_BOTTOMRIGHT = 7,
	TILE_BOTTOM = 8,
	TILE_BOTTOMLEFT = 9,
	TILE_LEFT = 10,
	TILE_TOPLEFT = 11
};

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType, Sprite* graphics);

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();
private:
	//Currently used sprite
	Sprite* m_Sprite;

	//The collision box
	SDL_Rect m_Box;

	//The tile type
	int m_Type;
};

