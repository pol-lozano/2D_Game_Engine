#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"
#include "../TextureHandler.h"

struct ColliderComponent : public Component 
{
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	ColliderComponent(std::string t) 
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override 
	{
		//Add transform component if it doesn't already have one
		if (!entity->hasComponent<TransformComponent>()) 
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureHandler::loadFromFile("assets/coltex.png");

		srcR = { 0,0,32,32 };
		destR = { collider.x,collider.y,collider.w,collider.h };
	}

	void update() override
	{
		//Static collider
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destR.x = collider.x - Core::camera.x;
		destR.y = collider.y - Core::camera.y;
	}

	void draw() override 
	{
		TextureHandler::draw(tex, srcR, destR, SDL_FLIP_NONE);
	}

};