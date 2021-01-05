#pragma once
#include "../Component.h"
#include "../../Core/Core.h"
#include <string>
#include "Rigidbody2D.h"

class BoxCollider2D : public Component{
public:
	BoxCollider2D(SDL_Renderer* target, int width, int height) : rTarget(target), size{width, height} {}
	BoxCollider2D(SDL_Renderer* target, int width, int height, std::string tag) : rTarget(target), collisionTag(tag), size{width, height} {}

	void init() override final {
		transform = &entity->getComponent<Transform>();
		if (!entity->hasComponent<Rigidbody2D>()) { entity->addComponent<Rigidbody2D>(); }
		rb = &entity->getComponent<Rigidbody2D>();

		//Add to collider list 
		Core::get().addCollider(this);
	}

	void update(double dt) override final {
		const SDL_Rect visibleArea = Core::get().getVisibleArea();

		box.w = static_cast<int>(size.x * transform->scale.x);
		box.h = static_cast<int>(size.y * transform->scale.y);

		box.x = static_cast<int>(transform->position.x) - visibleArea.x;
		box.y = static_cast<int>(transform->position.y) - visibleArea.y;
	}

	void draw() override final {
		if (visible) {
			SDL_SetRenderDrawColor(rTarget, 255, 214, 85, 255);
			SDL_RenderDrawRect(rTarget, &box);
		}
	}

	Vec2F resolveOverlap(const SDL_Rect& overlap) {
		//Do not move object if its set as kinematic
		if (!transform->kinematic) return Vec2F();

		float resolve = 0;
		float xDiff = (box.x + (box.w * 0.5f)) - (overlap.x + (overlap.w * 0.5f)); 
		float yDiff = (box.y + (box.h * 0.5f)) - (overlap.y + (overlap.h * 0.5f));

		if (fabs(xDiff) > fabs(yDiff)) {
			//Resolve collision on the left or right
			resolve = (xDiff > 0) ? overlap.w : -(overlap.w);
			return Vec2F(resolve, 0); //transform->translateX(resolve);
		}
		else {
			//Resolve collision on the up or down
			resolve = (yDiff > 0) ? overlap.h : -(overlap.h);
			return Vec2F(0, resolve); //transform->translateY(resolve);
		}
	}

	inline void setVisible(bool v) { visible = v; }
	inline std::string getCollisionTag() const { return collisionTag; }
	inline SDL_Rect& getBox() { return box; }

private:
	friend class Collision;
	Transform* transform = nullptr;
	Rigidbody2D* rb = nullptr;

	SDL_Renderer* rTarget = nullptr;
	std::string collisionTag = "";

	bool visible = false;
	SDL_Point size = { 0, 0 };
	SDL_Rect box = { 0, 0, 0, 0 };
};