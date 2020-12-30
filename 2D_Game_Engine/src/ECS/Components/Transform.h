#pragma once
#include "../../Physics/Vec2.h"
#include "../Component.h"

struct Transform : public Component {
	Vec2F position = Vec2F();
	Vec2F scale = Vec2F(1, 1);
	float rotation = 0;
	bool kinematic = true;

	Transform(float x, float y) {
		position = Vec2F(x, y);
	}

	Transform(float x, float y, float scx, float scy) {
		position = Vec2F(x, y);
		scale = Vec2F(scx, scy);
	}

	Transform(float x, float y, float scx, float scy, float rot) {
		position = Vec2F(x, y);
		scale = Vec2F(scx, scy);
		rotation = rot;
	}

	inline void translate(const Vec2F& v) {
		position += v;
	}

	inline void translateX(const float f) {
		position.x += f;
	}

	inline void translateY(const float f) {
		position.y += f;
	}

	//Not sure if unity does it like this??
	inline void moveTowards(const Vec2F dest, float time) {
		position.lerp(position, dest, time);
	}
};