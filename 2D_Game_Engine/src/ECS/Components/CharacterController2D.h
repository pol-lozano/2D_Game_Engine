#pragma once
#include "../Entity.h"
#include "../Component.h"

#include "InputHandler.h"

class CharacterController2D : public Component {
public:
	void init() override final {
		transform = &entity->getComponent<Transform>();
		//Ensure entity has a rigidbody2D and InputHandler
		if (!entity->hasComponent<Rigidbody2D>()) { entity->addComponent<Rigidbody2D>(); }
		if (!entity->hasComponent<InputHandler>()) { entity->addComponent<InputHandler>(); }

		rb = &entity->getComponent<Rigidbody2D>();
		input = &entity->getComponent<InputHandler>();
	}

	void update(double dt) override final {	
		//move(input->getInputRaw());	
		move(input->getInput());
	}

	void move(Vec2F dir) {
		//Clamp input
		if (dir.length() > 1) { dir = dir.normalize(); }
		//Invert speed for y axis
		rb->setForce(Vec2F(dir.x * speed, dir.y * -speed));
	}

private:
	Transform* transform;
	Rigidbody2D* rb;
	Sprite* sr;
	InputHandler* input;

	float speed = 300; //speed in pixels per sec
};
