#ifndef __Vec2_H__
#define __Vec2_H__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <class T>
struct Vec2 {
	T x, y;

	Vec2() :x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}

	Vec2& operator=(const Vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	//Addition
	Vec2 operator+(Vec2& v) {
		return Vec2(x + v.x, y + v.y);
	}

	Vec2 operator+(T s) {
		return Vec2(x + s, y + s);
	}

	Vec2& operator+=(Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2& operator+=(T s) {
		x += s;
		y += s;
		return *this;
	}

	//Subtraction
	Vec2 operator-(Vec2& v) {
		return Vec2(x - v.x, y - v.y);
	}

	Vec2 operator-(T s) {
		return Vec2(x - s, y - s);
	}

	Vec2& operator-=(Vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2& operator-=(T s) {
		x -= s;
		y -= s;
		return *this;
	}

	//Multiplication
	Vec2 operator*(T s) {
		return Vec2(x * s, y * s);
	}
	Vec2& operator*=(T s) {
		x *= s;
		y *= s;
		return *this;
	}

	//Division
	Vec2 operator/(T s) {
		return Vec2(x / s, y / s);
	}
	Vec2& operator/=(T s) {
		x /= s;
		y /= s;
		return *this;
	}

	void set(T x, T y) {
		this->x = x;
		this->y = y;
	}

	void rotate(double deg) {
		double theta = deg / 180.0 * M_PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	Vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	float dist(Vec2 v) const {
		Vec2 d(v.x - x, v.y - y);
		return d.length();
	}

	float length() const {
		return std::sqrt(x * x + y * y);
	}

	void truncate(double length) {
		double angle = atan2f(y, x);
		x = length * cos(angle);
		y = length * sin(angle);
	}

	Vec2 ortho() const {
		return Vec2(y, -x);
	}

	static float dot(Vec2 v1, Vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	static float cross(Vec2 v1, Vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	Vec2& lerp(Vec2 source, const Vec2 target, float time) {
		x = source.x + time * (target.x - source.x);
		y = source.y + time * (target.y - source.y);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vec2& v) {
		stream << "(" << v.x << "," << v.y << ")";
		return stream;
	}
};

typedef Vec2<float> Vec2F;

#endif