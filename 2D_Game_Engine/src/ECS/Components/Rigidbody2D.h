#pragma once
#include "../Entity.h"
#include "../Component.h"

constexpr float UNI_MASS = 1.0f;
constexpr float GRAVITY = 0; // 9.8f;

class Rigidbody2D : public Component {
public:
	Rigidbody2D() = default;
	virtual ~Rigidbody2D() = default;
	
	void init() override final {
		transform = &entity->getComponent<Transform>();
	}

	void update(double dt) override final {
		m_acceleration.x = (m_force.x + m_friction.x) / m_mass;
		m_acceleration.y = (m_gravity + m_force.y) / m_mass;
		m_velocity = m_acceleration * dt;
		//std::cout << m_velocity << " " << m_acceleration.x <<" " << m_acceleration.y << " " << dt;
		transform->translate(m_velocity);
	}

	void setForce(Vec2F f) {
		m_force = f;
	}

private:
	Transform* transform = nullptr;

	float m_mass = UNI_MASS;
	float m_gravity = GRAVITY;

	Vec2F m_force = Vec2F();
	Vec2F m_friction = Vec2F();

	Vec2F m_velocity = Vec2F();
	Vec2F m_acceleration = Vec2F();
};