#pragma once

#include "../Entity.h"
#include "../../Physics/Vector2.h"
#include "../Component.h"

constexpr float GRAVITY = 10.0f;

class Rigidbody2D : public Component {
public:
	Rigidbody2D() = default;
	virtual ~Rigidbody2D() = default;

	Rigidbody2D(float gravity_scale) : gravityScale(gravity_scale) {}
	
	bool init() override final {
		transform = &entity->getComponent<Transform>();
		return true;
	}

	void update() override final {
		velocity.x = force.x - drag.x;
		velocity.y = force.y - drag.y + gravityScale * GRAVITY;
		transform->translate(velocity);
	}

	void setForce(const Vector2 f) {
		force = f;
	}

private:
	float gravityScale = 1.0f;
	Vector2 drag = Vector2();
	Vector2 force = Vector2();

	Vector2 velocity = Vector2();
	Transform* transform = nullptr;
};