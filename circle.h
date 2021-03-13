#ifndef CIRCLE_H
#define CIRCLE_H
#include "vec.h"
#include "intersectData.h"

const float MAX_VEL = 25.0f;
const float DEACC = 0.1f;

class Circle {
public:
	Vec2f center;
	Vec2f vel;
	Vec2f force;
	float radius;
	bool onGround;

	Circle();
	Circle(Vec2f point, const float r);
	Circle(float x, float y, const float r);
	void init(float x, float y, const float r);
	void operator=(Circle c);
	void addForce(float x, float y);
	float round(float var);
	IntersectData intersectingCircle(Circle& c);
	IntersectData intersectingPoint(float x, float y);
	void move(float xbounds, float ybounds, bool toggleGravity=false, float gravity=0.05);
};

#endif
