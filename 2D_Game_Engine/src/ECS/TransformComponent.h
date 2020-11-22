#pragma once
#include "Components.h"
#include "../Vector2.h"

struct TransformComponent : public Component 
{
	Vector2 position;
	Vector2 velocity;

	int speed = 3;


	TransformComponent() {
		position.x = 0;
		position.y = 0;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};