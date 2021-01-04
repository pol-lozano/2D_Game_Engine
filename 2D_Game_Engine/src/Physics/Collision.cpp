#include "Collision.h"

//Axis aligned bounding box
/*
bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b, SDL_Rect& result)
{
    return(a.x + a.w >= b.x && b.x + b.w >= a.x &&
           a.y + a.h >= b.y && b.y + b.h >= a.y);
}*/

bool Collision::AABB(const SDL_Rect* a, const SDL_Rect* b, SDL_Rect* result) { 
    return SDL_IntersectRect(a, b, result);
}

bool Collision::AABB(const BoxCollider2D& a, const BoxCollider2D& b, SDL_Rect& result) {
    return AABB(&a.box, &b.box, &result);
}

 