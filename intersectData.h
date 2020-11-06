#pragma once

class IntersectData {
	private:
	const bool intersecting;
	const float distance;
	public:
	IntersectData(const bool i, const float d) :
		intersecting(i), distance(d) {}

	const bool getIntersecting() { return intersecting; }
	const float getDistance() { return distance; }
};
