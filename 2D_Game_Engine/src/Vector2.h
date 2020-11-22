#pragma once
#include <iostream>

class Vector2
{
public:
	float x;
	float y;
	Vector2();
	Vector2(float x, float y);

	Vector2& Add(const Vector2& v);
	Vector2& Subtract(const Vector2& v);
	Vector2& Multiply(const Vector2& v);
	Vector2& Divide(const Vector2& v);

	friend Vector2& operator+(Vector2& v1, const Vector2& v2);
	friend Vector2& operator-(Vector2& v1, const Vector2& v2);
	friend Vector2& operator*(Vector2& v1, const Vector2& v2);
	friend Vector2& operator/(Vector2& v1, const Vector2& v2);

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator/=(const Vector2& v);

	friend std::ostream& operator<<(std::ostream& stream, const Vector2& v);
};
