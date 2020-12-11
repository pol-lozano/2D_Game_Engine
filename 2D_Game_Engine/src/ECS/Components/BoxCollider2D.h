#pragma once
#include <SDL.h>
#include <string>
#include "../Entity.h"
#include "../Component.h"
#include "../../Core/Core.h"

class BoxCollider2D : public Component{
public:
	BoxCollider2D() = default;
	~BoxCollider2D() = default;

	BoxCollider2D(SDL_Renderer* target, int width, int height) : rTarget(target) {
		size = { width, height };
	}

	BoxCollider2D(SDL_Renderer* target, int width, int height, std::string tag) : rTarget(target), collisionTag(tag) {
		size = { width, height };
	}

	bool init() override final {
		transform = &entity->getComponent<Transform>();
		//Add to collider list 
		Core::get().addCollider(this);
		return true;
	}

	void update(float dt) override final {
		box.w = static_cast<int>(size.x * transform->scale.x);
		box.h = static_cast<int>(size.y * transform->scale.y);

		box.x = static_cast<int>(transform->position.x) - Core::get().getCamera()->x;
		box.y = static_cast<int>(transform->position.y) - Core::get().getCamera()->y;
	}

	void draw() override final {
		if (visible) {
			SDL_SetRenderDrawColor(rTarget, 255, 214, 85, 255);
			//SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
			SDL_RenderDrawRect(rTarget, &box);
		}
	}

	inline void setVisible(bool f) {
		visible = f;
	}

	inline std::string getCollisionTag() const {
		return collisionTag;
	}

private:
	friend class Collision;
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
	std::string collisionTag = "";

	bool visible = false;
	SDL_Point size = { 0, 0 };
	SDL_Rect box = { 0, 0, 0, 0 };
};