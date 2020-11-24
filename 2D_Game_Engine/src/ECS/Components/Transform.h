#pragma once
#include "../../Physics/Vector2.h"
#include "../Component.h"

class Transform : public Component {
public:
	Vector2 position = Vector2();
	Vector2 scale = Vector2(1,1);
	float rotation = 0;

	Transform() = default;
	virtual ~Transform() = default;

	Transform(float x, float y) {
		position = Vector2(x,y);
	}

	Transform(float x, float y, float scx, float scy) {
		position = Vector2(x, y);
		scale = Vector2(scx, scy);
	}

	Transform(float x, float y, float scx,float scy, float rot) {
		position = Vector2(x, y);
		scale = Vector2(scx, scy);
		rotation = rot;
	}

	void translate(const Vector2 v) {
		position += v;
	}

	//Not sure if unity does it like this??
	void moveTowards(const Vector2 dest, float time) {
		position.lerp(position, dest, time);
	}

};