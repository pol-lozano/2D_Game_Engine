#pragma once
#include "../Core.h"
#include "ECS.h"
#include "Components.h"

struct KeyboardController : public Component 
{
	TransformComponent* transform;
	void init() override 
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override
	{
		if (Core::event.type == SDL_KEYDOWN)
		{
			switch(Core::event.key.keysym.sym)
			{
				case SDLK_UP:
					transform->velocity.y = -1;
					break;
				case SDLK_DOWN:
					transform->velocity.y = 1;
					break;
				case SDLK_LEFT:
					transform->velocity.x = -1;
					break;
				case SDLK_RIGHT:
					transform->velocity.x = 1;
					break;
				default:
					break;
			}
		}
		if (Core::event.type == SDL_KEYUP)
		{
			switch (Core::event.key.keysym.sym)
			{
			case SDLK_UP:
				transform->velocity.y = 0;
				break;
			case SDLK_DOWN:
				transform->velocity.y = 0;
				break;
			case SDLK_LEFT:
				transform->velocity.x = 0;
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 0;
				break;
			default:
				break;
			}
		}
	}
};