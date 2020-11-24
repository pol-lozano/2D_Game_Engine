#pragma once
#include <iostream>

struct Vector2
{
	float x, y;

	Vector2(){
		this->x = 0;
		this->y =   0;
	}

	Vector2(float x, float y){
		this->x = x;
		this->y = y;
	}

	inline Vector2 operator+(const Vector2& v2) const {
		return Vector2(x + v2.x, y + v2.y);
	}

	inline friend Vector2& operator+=(Vector2& v1, const Vector2& v2) {
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	inline Vector2 operator-(const Vector2& v2) const {
		return Vector2(x - v2.x, y - v2.y);
	}

	inline friend Vector2& operator-=(Vector2& v1, const Vector2& v2) {
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	inline Vector2 operator*(const float& scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	inline Vector2 operator/(const float& d) const {
		return (d != 0 ? Vector2(x/d, y/d) : Vector2());
	}
	
	inline Vector2& zero() {
		this->x = 0;
		this->y = 0;
		return *this;
	}

	inline Vector2& one() {
		this->x = 1;
		this->y = 1;
		return *this;
	}
	inline Vector2& lerp(Vector2 source, Vector2 target, float time) {
		this->x = source.x + time * (target.x - source.x);
		this->y = source.y + time * (target.y - source.y);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2& v) {
		stream << "(" << v.x << "," << v.y << ")";
		return stream;
	}

};
