#pragma once
#include <SDL.h>
#include "Sprite.h"

enum ButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class Button
{
public:
	Button(Sprite* graphics);
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);
	void render(SDL_Renderer* renderer);
private:
	//Currently used sprite
	Sprite* m_Sprite;

	//Top left position;
	SDL_Point m_Pos;

	int m_Width;
	int m_Height;
};

