#pragma once
#include "../Component.h"

constexpr float MASS = 1;
constexpr float GRAVITY = 0;

class Rigidbody2D : public Component {
public:
	void init() override final {
		transform = &entity->getComponent<Transform>();
	}

	void update(double dt) override final {
		acceleration.x = (force.x + friction.x) / mass;
		acceleration.y = (gravity + force.y) / mass;
		velocity = acceleration * dt;
		transform->translate(velocity);
	}

	void setForce(Vec2F f) { force = f; }
	void setGravity(float g) { gravity = g; }

private:
	Transform* transform = nullptr;

	float mass = MASS;
	float gravity = GRAVITY;

	Vec2F force = Vec2F();
	Vec2F friction = Vec2F();
	Vec2F velocity = Vec2F();
	Vec2F acceleration = Vec2F();
};