#pragma once
#include <SDL.h>
#include "Sprite.h"
#include "Tile.h"

class Entity
{
public:
	//Constructor
	Entity(Sprite* graphics);

	//Take in key presses and adjusts the entities's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and check collision against tiles
	void move(Tile* tiles[], int w, int h, int tilemap_Size);

	//Centers the camera over the dot
	void setCamera(SDL_Rect& camera, int w, int h, int levelWidth, int levelHeight);

	//Shows the dot on the screen
	void render(SDL_Rect& camera);

private:
	//Sprite which holds the entities graphics
	Sprite* m_Graphics;

	//Collision box
	SDL_Rect m_Box;
	//Maximum axis velocity of the entity
	int m_Speed = 10;
	//Velocity of the entity
	int m_VelX, m_VelY;
};

