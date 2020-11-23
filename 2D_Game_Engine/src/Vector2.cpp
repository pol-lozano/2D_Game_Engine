#include "Vector2.h"

Vector2::Vector2()
{
    x = 0.0f;
    x = 0.0f;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2& Vector2::add(const Vector2& v)
{
    this->x += v.x;
    this->y += v.y;
    return *this;
}

Vector2& Vector2::subtract(const Vector2& v)
{
    this->x -= v.x;
    this->y -= v.y;
    return *this;
}

Vector2& Vector2::multiply(const Vector2& v)
{
    this->x *= v.x;
    this->y *= v.y;
    return *this;
}

Vector2& Vector2::divide(const Vector2& v)
{
    this->x /= v.x;
    this->y /= v.y;
    return *this;
}

Vector2& operator+(Vector2& v1, const Vector2& v2)
{
    return v1.add(v2);
}

Vector2& operator-(Vector2& v1, const Vector2& v2)
{
    return v1.subtract(v2);
}

Vector2& operator*(Vector2& v1, const Vector2& v2)
{
    return v1.multiply(v2);
}

Vector2& operator/(Vector2& v1, const Vector2& v2)
{
    return v1.divide(v2);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    return this->add(v);
}

Vector2& Vector2::operator-=(const Vector2& v)
{
    return this->subtract(v);
}

Vector2& Vector2::operator*=(const Vector2& v)
{
    return this->multiply(v);
}

Vector2& Vector2::operator/=(const Vector2& v)
{
    return this->divide(v);
}

Vector2& Vector2::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2& Vector2::zero()
{
    this->x = 0.0f;
    this->y = 0.0f;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << "," << v.y << ")";
    return os;
}
