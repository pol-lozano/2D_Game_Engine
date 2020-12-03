#pragma once
#include "../Entity.h"
#include "../../Physics/Vector2.h"
#include "../Component.h"
#include "../../Core/Core.h"
#include "InputHandler.h"

struct CharacterController2D : public Component {
	Transform* transform;
	Rigidbody2D* rb;
	InputHandler* input;
	float speed = 200; //speed in pixels per sec

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
		Vector2 dir = input->getInputRaw().normalized();
		std::cout << dir << std::endl;
		//Invert speed for y axis
		rb->setForce(Vector2(dir.x * speed, dir.y * -speed));
	}
};
