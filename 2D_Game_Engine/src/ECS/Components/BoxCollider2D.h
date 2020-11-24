#pragma once
#include <SDL.h>
#include <string>
#include "../Entity.h"
#include "../Component.h"

class BoxCollider2D : public Component{
public:
	BoxCollider2D(SDL_Renderer* target, int width, int height) : rTarget(target) {
		box.w = width;
		box.h = height;
	}

	BoxCollider2D(SDL_Renderer* target, int width, int height,std::string tag) :rTarget(target), collisionTag(tag) {
		box.w = width;
		box.h = height;
	}
	~BoxCollider2D() = default;

	bool init() override final {
		transform = &entity->getComponent<Transform>();
		return true;
	}

	void draw() override final {
		SDL_SetRenderDrawColor(rTarget, 255, 214, 85, 255);
		SDL_RenderDrawRect(rTarget, &box);
	}

	void update(float dt) override final {
		box.x = static_cast<int>(transform->position.x);
		box.y = static_cast<int>(transform->position.y);
	}

	std::string getCollisionTag() const {
		return collisionTag;
	}

private:
	friend class Collision;
	SDL_Rect box = { 0,0,0,0 };
	std::string collisionTag = "";
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
};