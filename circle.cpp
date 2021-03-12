#include "circle.h"

void swap(float& a, float& b) {
	float tmp = a;
	a = b;
	b = tmp;
}

Circle::Circle() : radius(10.0f)
{
    center = Vec2f (50.0f, 50.0f);
    onGround = false;
}
Circle::Circle(Vec2f point, const float r) : radius(r), center(point) {}
Circle::Circle(float x, float y, const float r) : radius(r)
{
    center.x = x;
    center.y = y;
    onGround = false;
}
void Circle::operator=(Circle c)
{
    this->center = c.center;
    this->radius = c.radius;
}
void Circle::addForce(float x, float y)
{
    force.x += x;
    force.y += y;
}
float Circle::round(float var)
{
    float value = (int)(var * 100 + .5);
    var = (float)value / 100;
    return var;
}
// Collision
IntersectData Circle::intersectingCircle(Circle& c)
{
    float radiusDist = this->radius + c.radius;
    float scalar = 0.05f;
    Vec2f slope = this->center - c.center;
    float centerDist = slope.length;

    if(radiusDist >= centerDist) {
        if(this->radius == c.radius) {
            // Same mass, just swap velocities
            this->vel = this->vel * scalar;
            c.vel = c.vel * scalar;
            swap(this->vel.x, c.vel.x);
            swap(this->vel.y, c.vel.y);
        } else {

        }
        // Push them out  of each other's radii
        addForce(slope.x * scalar, slope.y * scalar);
        c.addForce(-slope.x * scalar, -slope.y * scalar);
        return IntersectData(true, centerDist - radiusDist);
    }
    return IntersectData(false, centerDist - radiusDist);
}
IntersectData Circle::intersectingPoint(float x, float y)
{
    float xdiff = center.x - x;
    float ydiff = center.y - y;
    float dist = sqrt(xdiff*xdiff + ydiff*ydiff);
    float forcex = xdiff * 0.2;
    float forcey = ydiff * 0.2;

    if(radius >= dist) {
        force.x += forcex;
        force.y += forcey;
        return IntersectData(true, dist);
    }
    else
        return IntersectData(false, dist);
}
// Movement
void Circle::move(float xbounds, float ybounds, bool toggleGravity, float gravity)
{
    if(toggleGravity) {
        force.y += gravity;
    }

    vel.x += force.x;
    vel.y += force.y;
    // Velocity checks
    if(abs(vel.x) <= DEACC) vel.x = 0.0f;
    if(abs(vel.y) <= DEACC) vel.y = 0.0f;
    if(abs(vel.x) > MAX_VEL) vel.x = vel.x > 0 ? MAX_VEL : -MAX_VEL;
    if(abs(vel.y) > MAX_VEL) vel.y = vel.y > 0 ? MAX_VEL : -MAX_VEL;

    // Erase all force
    force.x = force.y = 0.0;
    // Move within bounds
    if(vel.x != 0.0f) {
        if(center.x + vel.x + radius >= xbounds) {
        center.x -= (center.x + vel.x + radius) - xbounds;
        vel.x *= -1;
        } else if(center.x + vel.x - radius <= 0) {
            center.x += 0 - (center.x + vel.x - radius);
            vel.x *= -1;
        } else {
            center.x += vel.x;
        }
    }
    if(vel.y != 0.0f) {
        if(center.y + vel.y + radius >= ybounds) {
            vel.y -= (center.y + vel.y + radius) - ybounds;
            if(round(vel.y) == -round(gravity)) {
                onGround = true;
                vel.y = 0.0f;
                center.y = ybounds - radius;
            } else {
                center.y -= vel.y;
                vel.y *= -1;
            }
        } else if(center.y + vel.y - radius <= 0) {
            vel.y += 0 - (center.y + vel.y - radius);
            center.y += vel.y;
            vel.y *= -1;
        } else {
            center.y += vel.y;
        }
    }
    // Deacceleration
    float deaccX = vel.x > 0 ? -DEACC : DEACC;
    float deaccY = vel.y > 0 ? -DEACC : DEACC;
    if(toggleGravity && deaccY > 0) {
        // Only deaccelerate if going up
        vel.y += deaccY;
    } else {
        vel.y += deaccY;
    }
    if(vel.x != 0) {
        vel.x += deaccX;
    }
    
    if(center.y < 0 || center.y > ybounds || center.x < 0 || center.x > xbounds) {
        center.x = xbounds / 2.0f;
        center.y = -radius;
    }
}