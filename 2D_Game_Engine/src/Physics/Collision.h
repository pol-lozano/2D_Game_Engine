#pragma once
#include "../ECS/Components/BoxCollider2D.h"

//Different types of collision detection
struct Collision
{
	//Access aligned bounding box collision
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const BoxCollider2D& a, const BoxCollider2D& b);
};
