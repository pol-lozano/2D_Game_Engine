#pragma once
class Entity;

class Component {
public:
	//Components owner
	Entity* entity = nullptr;

	virtual void init() {}
	virtual void update(double dt) {}
	virtual void draw() {}
};