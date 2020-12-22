#pragma once
#include "../../Physics/vec2.h"
#include "../Component.h"

struct Transform : public Component {
	Vec2F position = Vec2F();
	Vec2F scale = Vec2F(1, 1);
	float rotation = 0;

	Transform() = default;
	virtual ~Transform() = default;

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

	void translate(Vec2F& v) {
		position += v;
	}

	void translateX(const float f) {
		position.x += f;
	}

	void translateY(const float f) {
		position.y += f;
	}

	//Not sure if unity does it like this??
	void moveTowards(const Vec2F dest, float time) {
		position.lerp(position, dest, time);
	}

};