#pragma once
#include <iostream>

class Vector2
{
public:
	float x;
	float y;
	Vector2();
	Vector2(float x, float y);

	Vector2& add(const Vector2& v);
	Vector2& subtract(const Vector2& v);
	Vector2& multiply(const Vector2& v);
	Vector2& divide(const Vector2& v);

	friend Vector2& operator+(Vector2& v1, const Vector2& v2);
	friend Vector2& operator-(Vector2& v1, const Vector2& v2);
	friend Vector2& operator*(Vector2& v1, const Vector2& v2);
	friend Vector2& operator/(Vector2& v1, const Vector2& v2);

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator/=(const Vector2& v);

	Vector2& operator*(const int& i);
	//Set vector to 0
	Vector2& zero();



	friend std::ostream& operator<<(std::ostream& stream, const Vector2& v);
};
