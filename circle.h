#pragma once
#include "vec.h"
#include "intersectData.h"

const float MAX_VEL = 50.0f;

class Circle {
	private:
	Vec2f center;
	float radius;
	float angle = 0.0f;
	const float deacc = 0.15f;
	float dx = 0.0f;
	float dy = 0.0f;
	Vec2f bounds;
	bool onGround = false;
	
	public:

	Circle() : radius(10.0f)
	{
		center = Vec2f (50.0f, 50.0f);
	}
	Circle(Vec2f point, const float r) : radius(r), center(point) {}
	Circle(float x, float y, const float r) : radius(r)
	{
		center.x = x;
		center.y = y;
	}
	float getDx() { return dx; }
	float getDy() { return dy; }
	bool getOnGround() { return onGround; }
	void setDx(float newDx) { dx = newDx; }
	void setDy(float newDy) { dy = newDy; }
	Vec2f getCenter() { return center; }
	const float getRadius() { return radius; }
	void setCenter(Vec2f v) { center = v; }
	void operator=(Circle c)
	{
		this->center = c.getCenter();
		this->radius = c.getRadius();
	}
	// Collision
	IntersectData intersectingCircle(Circle& c)
	{
		float radiusDist = this->radius + c.getRadius();
		float centerDist = (this->center - c.getCenter()).length;

		if(centerDist < radiusDist) {
			float penalty = centerDist-radiusDist;
			float cradius = c.getRadius();
			Vec2f ccenter = c.getCenter();
			//float maxR = cradius > radius ? cradius : radius;
			if(center.x < ccenter.x) {
				center = center + Vec2f (penalty, penalty);
				c.setCenter(ccenter - Vec2f (penalty, penalty));
			} else {
				center = center - Vec2f (penalty, penalty);
				c.setCenter(ccenter + Vec2f (penalty, penalty));
			}
			float cdx = c.getDx();
			float cdy = c.getDy();
			float tmpx = dx;
			float tmpy = dy;
			if(cradius == radius) {
				c.setDx(tmpx);
				c.setDy(tmpy);
				dx = cdx;
				dy = cdy;
			}
			else {
				float newDx1 = ((radius-cradius)/(radius+cradius))*tmpx +
					((2*cradius)/(radius+cradius))*cdx;
				float newDx2 = ((2*radius)/(radius+cradius))*tmpx +
					((cradius-radius)/(radius+cradius))*cdx;
				float newDy1 = ((radius-cradius)/(radius+cradius))*tmpy +
					((2*cradius)/(radius+cradius))*cdy;
				float newDy2 = ((2*radius)/(radius+cradius))*tmpy +
					((cradius-radius)/(radius+cradius))*cdy;
				dx = newDx1;
				dy = newDy1;
				c.setDx(newDx2);
				c.setDy(newDy2);
			}
			if(center.x >= bounds.x && center.y >= bounds.y) {
				center.y -= bounds.y / 10;
				center.x -= bounds.x / 10;
			} else if(center.x <= 0 && center.y >= bounds.y) {
				center.y -= bounds.y / 10;
				center.x += bounds.x / 10;
			}
			//c.setDx(cdx);
			//c.setDy(cdy);
			//dx *= penalty * (maxR/cradius);
			//dy *= penalty * (maxR/cradius);
			return IntersectData(true, centerDist - radiusDist);
		}
		else
			return IntersectData(false, centerDist - radiusDist);
	}
	IntersectData intersectingPoint(float x, float y)
	{
		float xdiff = center.x - x;
		float ydiff = center.y - y;
		float dist = sqrt(xdiff*xdiff + ydiff*ydiff);
		float forcex = xdiff * 0.2;
		float forcey = ydiff * 0.2;

		if(radius >= dist) {
			dx += forcex;
			dy += forcey;
			return IntersectData(true, dist);
		}
		else
			return IntersectData(false, dist);
	}
	// Movement
	void move(float xbounds, float ybounds, bool toggleGravity=false, float gravity=1.2)
	{
		if(dy < -0.5 || dy > 0.5) onGround = false;
		if((int)center.y < (int)ybounds-(int)radius-1) onGround = false;
		bounds = Vec2f (xbounds, ybounds);
		if(dy > MAX_VEL) dy = MAX_VEL;
		else if(dy < -MAX_VEL) dy = -MAX_VEL;
		if(dx > MAX_VEL) dx = MAX_VEL;
		else if(dx < -MAX_VEL) dx = -MAX_VEL;
		
		//if(!onGround) std::cout << this << ": " << dy << std::endl;
		if(dy == -0.497368f || dy == -0.497369f) onGround = true;

		if(toggleGravity) {
			if(!onGround)
				dy += gravity;
			else
				dy = 0.0f;
		}

		//std::cout << dy << std::endl;
		center += Vec2f (dx, dy);
		// Rolling forward/backward
		if(dx < 0) {
			dx += deacc;
			angle += deacc;
		} else if(dx > 0) {
			dx -= deacc;
			angle -= deacc;
		}
		// Falling down/gravity
		if(dy < 0) {
			dy += deacc;
		} else if(dy > 0) {
			dy -= deacc;
		}

		// Check if on screen
		if(center.x-radius < 0) {
			center.x = radius;
			dx *= -1;
		} else if(center.x+radius > xbounds) {
			center.x = xbounds - radius;
			dx *= -1;
		}

		if(center.y-radius < 0) {
			center.y = radius;
			dy *= -1.6f;
		} else if(center.y+radius >= ybounds) {
			center.y = ybounds - radius;
			if(dy < 0.5f && dy > -0.5f) {
				onGround = true;
				dy = 0.0;
			}
			dy *= -1;
		}
	}
};
