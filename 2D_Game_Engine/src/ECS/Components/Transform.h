#pragma once
#include "../Component.h"
#include "../../Physics/Vec2.h"

struct Transform : public Component {
	Transform(float x, float y) : position{ x, y } {}
	Transform(float x, float y, float scx, float scy) : position{ x, y }, scale{ scx, scy } {}
	Transform(float x, float y, float scx, float scy, float rot) : position{ x, y }, scale{ scx, scy }, rotation(rot) {}

	//Translate methods
	inline void translate(const Vec2F& v) { position += v; }
	inline void translateX(const float f) { position.x += f; }
	inline void translateY(const float f) { position.y += f; }

	//Not sure if unity does it like this??
	inline void moveTowards(const Vec2F dest, float time) {
		position.lerp(position, dest, time);
	}

	Vec2F position = Vec2F();
	Vec2F scale = Vec2F(1, 1);
	float rotation = 0;
	bool kinematic = true;
};