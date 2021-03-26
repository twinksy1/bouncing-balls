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
    this->vel = c.vel;
    this->force = c.force;
    this->onGround = c.onGround;
}
void Circle::addForce(float x, float y)
{
    force.x += x;
    force.y += y;
}
float Circle::round(float var)
{
    int value = (int)(var * 100);
    var = value / 100;
    return (float)var;
}
void Circle::init(float x, float y, const float r)
{
    radius = r;
    center.x = x;
    center.y = y;
    onGround = false;
}
// Collision
IntersectData Circle::intersectingCircle(Circle& c)
{
    float radiusDist = this->radius + c.radius;
    float scalar = 0.01f;
    Vec2f slope = this->center - c.center;
    float centerDist = slope.length;

    if(radiusDist >= centerDist) {
        if(this->radius == c.radius) {
            vel = vel * scalar;
            c.vel = c.vel * scalar;
            // Same mass, just swap velocities
            swap(this->vel.x, c.vel.x);
            swap(this->vel.y, c.vel.y);
            // Push them out  of each other's radii
            addForce(slope.x * scalar, slope.y * scalar);
            c.addForce(-slope.x * scalar, -slope.y * scalar);
        } else {
            // Different masses, use physics formula
            Vec2f v1i = this->vel * scalar;
            Vec2f v2i = c.vel * scalar;

            Vec2f v1f;
            Vec2f v2f;
            // New velocity of first mass
            v1f.x = (this->radius - c.radius)/(c.radius + this->radius) * v1i.x + (2 * c.radius)/(c.radius + this->radius) * v2i.x;
            v1f.y = (this->radius - c.radius)/(c.radius + this->radius) * v1i.y + (2 * c.radius)/(c.radius + this->radius) * v2i.y;
            // New velocity of second mass
            v2f.x = (2 * this->radius)/(c.radius + this->radius) * v1i.x + (c.radius - this->radius)/(c.radius + this->radius) * v2i.x;
            v2f.y = (2 * this->radius)/(c.radius + this->radius) * v1i.y + (c.radius - this->radius)/(c.radius + this->radius) * v2i.y;
            // Set new velocities
            this->vel = v1f;
            c.vel = v2f;
            float scalarx = 0.02f;
            float scalary = 0.05f;
            float ratio1 = (this->radius / c.radius);
            float ratio2 = (c.radius / this->radius);
            addForce(slope.x * ratio2 * scalarx, slope.y * ratio2 * scalary);
            c.addForce(-slope.x * ratio1 * scalarx, -slope.y * ratio1 * scalary);
        }
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
    if(center.x + vel.x + radius >= xbounds) {
    center.x -= (center.x + vel.x + radius) - xbounds;
    vel.x *= -1;
    } else if(center.x + vel.x - radius <= 0) {
        center.x += 0 - (center.x + vel.x - radius);
        vel.x *= -1;
    } else {
        center.x += vel.x;
    }


    if(center.y + vel.y + radius >= ybounds) {
        if(toggleGravity) {
            if(vel.y <= gravity) {
                center.y = ybounds - radius;
                vel.y = 0.0f;
                onGround = true;
            } else {
                center.y += vel.y;
                vel.y *= -1;
                onGround = false;
            }
        } else {
            onGround = false;
            center.y = ybounds - radius;
            vel.y *= -1;
            center.y += vel.y;
        }
    } else if(center.y + vel.y - radius <= 0) {
        onGround = false;
        center.y = radius;
        vel.y *= -1;
        center.y += vel.y;
    } else {
        onGround = false;
        center.y += vel.y;
    }

    // Deacceleration
    float deaccX = vel.x > 0 ? -DEACC : DEACC;
    float deaccY = vel.y > 0 ? -DEACC : DEACC;
    if(toggleGravity && deaccY > 0) {
        // Only deaccelerate if going up
        vel.y += deaccY;
    } else if(!toggleGravity) {
        vel.y += deaccY;
    }
    if(vel.x != 0) {
        vel.x += deaccX;
    }
    // Handle off screen
    if(center.y < 0 || center.y > ybounds || center.x < 0 || center.x > xbounds) {
        center.x = xbounds / 2.0f;
        center.y = -radius;
    }
}