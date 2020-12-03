#pragma once
#include "../ECS/Components/BoxCollider2D.h"

//Different types of collision detection
struct Collision
{
	//Axis aligned bounding box collision
	//static bool AABB(const SDL_Rect& a, const SDL_Rect& b, SDL_Rect& result);
	static bool AABB(const BoxCollider2D& a, const BoxCollider2D& b, SDL_Rect& result);
};
