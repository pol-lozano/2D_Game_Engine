#pragma once
#include "ECS.h"
#include <SDL.h>

struct TileComponent : public Component
{
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2 position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int posX, int posY, int tsize, int tscale, const char* path)
	{
		texture = TextureHandler::loadFromFile(path);
		position.x = posX;
		position.y = posY;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = posX;
		destRect.y = posY;
		destRect.w = destRect.h = tsize * tscale;
	}

	void update() override
	{
		destRect.x = position.x - Core::camera.x;
		destRect.y = position.y - Core::camera.y;
	}

	void draw() override
	{
		TextureHandler::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

};