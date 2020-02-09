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

	//Returns AABB that encompases both inputted boxes, used for moving scene elements
	static AABB SurroundingBox(AABB a, AABB b)
	{
		AA::Vec3 small(
			AA::dMin(a.Min().X(), b.Min().X()),
			AA::dMin(a.Min().Y(), b.Min().Y()),
			AA::dMin(a.Min().Z(), b.Min().Z())
		);

		AA::Vec3 big(
			AA::dMax(a.Max().X(), b.Max().X()),
			AA::dMax(a.Max().Y(), b.Max().Y()),
			AA::dMax(a.Max().Z(), b.Max().Z())
		);
		return AABB(small, big);
	}

private:
	AA::Vec3 _min;
	AA::Vec3 _max;
};

