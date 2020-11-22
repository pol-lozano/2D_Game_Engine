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

Vector2& Vector2::Add(const Vector2& v)
{
    this->x += v.x;
    this->y += v.y;
    return *this;
}

Vector2& Vector2::Subtract(const Vector2& v)
{
    this->x -= v.x;
    this->y -= v.y;
    return *this;
}

Vector2& Vector2::Multiply(const Vector2& v)
{
    this->x *= v.x;
    this->y *= v.y;
    return *this;
}

Vector2& Vector2::Divide(const Vector2& v)
{
    this->x /= v.x;
    this->y /= v.y;
    return *this;
}

Vector2& operator+(Vector2& v1, const Vector2& v2)
{
    return v1.Add(v2);
}

Vector2& operator-(Vector2& v1, const Vector2& v2)
{
    return v1.Subtract(v2);
}

Vector2& operator*(Vector2& v1, const Vector2& v2)
{
    return v1.Multiply(v2);
}

Vector2& operator/(Vector2& v1, const Vector2& v2)
{
    return v1.Divide(v2);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    return this->Add(v);
}

Vector2& Vector2::operator-=(const Vector2& v)
{
    return this->Subtract(v);
}

Vector2& Vector2::operator*=(const Vector2& v)
{
    return this->Multiply(v);
}

Vector2& Vector2::operator/=(const Vector2& v)
{
    return this->Divide(v);
}

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << "," << v.y << ")";
    return os;
}
