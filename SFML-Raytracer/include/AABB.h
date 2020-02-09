#pragma once
#include "Utilities.h"


class AABB
{
public:
	AABB() = default;
	AABB(const AA::Vec3& min, const AA::Vec3& max) : _min(min), _max(max) { }

	bool IntersectedRay(const AA::Ray& ray, double tMin, double tMax);
	inline AA::Vec3 Min() const { return _min; }
	inline AA::Vec3 Max() const { return _max; }

private:
	AA::Vec3 _min;
	AA::Vec3 _max;
};

