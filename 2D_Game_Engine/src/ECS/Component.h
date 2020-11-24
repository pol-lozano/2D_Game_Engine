#pragma once
class Entity;

class Component {
public:
	Component() = default;
	virtual ~Component() = default;

	//Components owner
	Entity* entity;

	virtual bool init() { return true; }
	virtual void update() {}
	virtual void draw() {}
};