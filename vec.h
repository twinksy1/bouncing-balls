#ifndef VEC_H
#define VEC_H

#include <iostream>
class Vec2f {
	public:
	float x;
	float y;
	float length;

	Vec2f();
	Vec2f(float x, float y);
	Vec2f(const Vec2f& v);
	Vec2f operator+(const Vec2f& v);
	void operator+=(const Vec2f& v);
	bool operator==(const Vec2f& v);
	Vec2f operator-(const Vec2f& v);
	Vec2f operator*(float scalar);
	float operator*(const Vec2f& v);
	void operator=(const Vec2f& v);
	friend std::ostream& operator<<(std::ostream& os, const Vec2f& v);
	void setLength();
};

#endif
