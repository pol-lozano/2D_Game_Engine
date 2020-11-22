#pragma once
#include "SDL.h"
#include "Components.h"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		setTexture(path);
	}

	void setTexture(const char* path) 
	{
		texture = TextureHandler::loadFromFile(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = srcRect.h = 20;
		destRect.w = destRect.h = 20;

	}
	void update() override
	{
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
	}
	void draw() override
	{
		TextureHandler::draw(texture, srcRect, destRect);
	}
};