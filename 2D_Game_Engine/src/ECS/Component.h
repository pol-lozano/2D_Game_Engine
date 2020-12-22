#pragma once
class Entity;

class Component {
public:
	Component() = default;
	virtual ~Component() = default;

	//Components owner
	Entity* entity;

	virtual void init() {}
	virtual void update(double dt) {}
	virtual void draw() {}
};