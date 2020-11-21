#pragma once
#include <SDL.h>
#include "../Tile.h"

class Collision
{
public:
	static bool checkCollision(SDL_Rect a, SDL_Rect b);
	static bool touchesWall(SDL_Rect box, Tile* tiles[], int tileCount);
};

