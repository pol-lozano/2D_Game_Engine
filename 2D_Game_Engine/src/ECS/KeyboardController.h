#pragma once
#include "../Core.h"
#include "ECS.h"
#include "Components.h"

struct KeyboardController : public Component 
{
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override 
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Core::event.type == SDL_KEYDOWN)
		{
			switch(Core::event.key.keysym.sym)
			{
				case SDLK_UP:
					transform->velocity.y = -1;
					sprite->play("Walk");
					break;
				case SDLK_DOWN:
					transform->velocity.y = 1;
					sprite->play("Walk");
					break;
				case SDLK_LEFT:
					transform->velocity.x = -1;
					sprite->play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_RIGHT:
					transform->velocity.x = 1;
					sprite->play("Walk");
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
				sprite->play("Idle");
				break;
			case SDLK_DOWN:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_LEFT:
				transform->velocity.x = 0;
				sprite->play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 0;
				sprite->play("Idle");
				break;
			case SDLK_ESCAPE:
					Core::isRunning = false;
					break;
			default:
				break;
			}
		}
	}
};