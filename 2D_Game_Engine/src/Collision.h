#pragma once
#include <SDL.h>

struct ColliderComponent;

//Different types of collision detection
struct Collision
{
	//Access aligned bounding box collision
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const ColliderComponent& a, const ColliderComponent& b);
};
