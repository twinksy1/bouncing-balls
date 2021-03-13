#include "vec.h"
Vec2f::Vec2f()
{
    x = 0.0f;
    y = 0.0f;
}
Vec2f::Vec2f(float x, float y)
{
    this->x = x;
    this->y = y;
    length = sqrt(x*x + y*y);
}
Vec2f::Vec2f(const Vec2f& v)
{
    this->x = v.x;
    this->y = v.y;
}

Vec2f Vec2f::operator+(const Vec2f& v)
{
    float xsum = this->x + v.x;
    float ysum = this->y + v.y;

    return Vec2f (xsum, ysum);
}
void Vec2f::operator+=(const Vec2f& v)
{
    this->x += v.x;
    this->y += v.y;
    length = sqrt(x*x + y*y);
}
bool Vec2f::operator==(const Vec2f& v)
{
    if(x == v.x && y == v.y && length == v.length)
        return true;
    else
        return false;
}
Vec2f Vec2f::operator-(const Vec2f& v)
{
    float xdiff = this->x - v.x;
    float ydiff = this->y - v.y;

    return Vec2f (xdiff, ydiff);
}
Vec2f Vec2f::operator*(float scalar)
{
    float scalarx = this->x * scalar;
    float scalary = this->y * scalar;

    return Vec2f (scalarx, scalary);
}
float Vec2f::operator*(const Vec2f& v)
{
    float newx = this->x * v.x;
    float newy = this->y * v.y;

    return newx + newy;
}
void Vec2f::operator=(const Vec2f& v)
{
    this->x = v.x;
    this->y = v.y;
    this->length = sqrt(this->x*this->x + this->y*this->y);
}
std::ostream& operator<<(std::ostream& os, const Vec2f& v)
{
    os << "x: " << v.x << ", y: " << v.y;
    return os;
}
void Vec2f::setLength()
{
    length = sqrt(x*x + y*y);
}