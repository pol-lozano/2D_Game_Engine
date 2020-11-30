#pragma once
#include "../Entity.h"
#include "../../Physics/Vector2.h"
#include "../Component.h"

constexpr float UNI_MASS = 1.0f;
constexpr float GRAVITY = 0;//9.8f;

class Rigidbody2D : public Component {
public:
	Rigidbody2D() {
		m_mass = UNI_MASS;
		m_gravity = GRAVITY;
		m_acceleration.zero();
		m_velocity.zero();
		m_force.zero();
		m_friction.zero();
	}
	
	bool init() override final {
		transform = &entity->getComponent<Transform>();
		return true;
	}

	void update(float dt) override final {
		m_acceleration.x = (m_force.x + m_friction.x) / m_mass;
		m_acceleration.y = m_gravity + m_force.y / m_mass;

		//Verlet integration
		//Vector2 curPos = transform->position;
		//transform->translate(curPos - transform->oldPosition + m_acceleration * (dt * dt));
		//transform->oldPosition = curPos;

		//Test
		m_velocity = m_acceleration * dt;
		transform->translate(m_velocity);
	}

	//Gravity & Mass
	inline void setMass(float mass) { m_mass = mass; }
	inline void setGravity(float gravity) { m_gravity = gravity; }

	//Force
	inline void setForce(const Vector2 f) { m_force = f; }
	inline void setForceX(const float fx) { m_force.x = fx; }
	inline void setForceY(const float fy) { m_force.y = fy; }
	inline void zeroForce() { m_force.zero(); }

	//Friction
	inline void setFriction(const Vector2 fr) { m_friction = fr; }
	inline void zeroFriction() { m_friction.zero(); }

	inline float getMass() { return m_mass; }
	inline Vector2 position() { return transform->position; }
	inline Vector2 velocity() { return m_velocity; }
	inline Vector2 acceleration() { return m_acceleration; }

private:
	Transform* transform = nullptr;

	float m_mass;
	float m_gravity;

	Vector2 m_force;
	Vector2 m_friction;

	Vector2 m_velocity;
	Vector2 m_acceleration;
};