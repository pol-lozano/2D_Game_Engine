#pragma once
#include "../Entity.h"
#include "../../Physics/vec2.h"
#include "../Component.h"
#include "../../Core/Core.h"
#include "InputHandler.h"

class CharacterController2D : public Component {
public:
	bool init() override final {
		transform = &entity->getComponent<Transform>();
		//Ensure entity has a rigidbody2D and InputHandler
		if (!entity->hasComponent<Rigidbody2D>()) { entity->addComponent<Rigidbody2D>(); }
		if (!entity->hasComponent<InputHandler>()) { entity->addComponent<InputHandler>(); }

		rb = &entity->getComponent<Rigidbody2D>();
		input = &entity->getComponent<InputHandler>();
		return true;
	}

	void update(float dt) override final
	{	
		Vec2F dir = input->getInputRaw().normalize();

		/*Vec2F dir = input->getInput();

		//Clamp input
		if (dir.length(dir) > 1) {
			dir = dir.normalize();
		}*/

		//Invert speed for y axis
		rb->setForce(Vec2F(dir.x * speed, dir.y * -speed));
	}

private:
	Transform* transform;
	Rigidbody2D* rb;
	InputHandler* input;
	float speed = 300; //speed in pixels per sec
};
