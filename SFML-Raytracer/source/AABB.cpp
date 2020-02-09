#include "..\include\AABB.h"

bool AABB::IntersectedRay(const AA::Ray& ray, double tMin, double tMax)
{
	// Using the slab method check if the ray is within each axis
	//If all axis are within and it lies between tMin and tMax of the ray then WE GOOD, INTERSECTED

	for (int i = 0; i < 3; i++)
	{
		//Lower side intersect
		double t0 = (Min()[i] - ray._startPos[i]) * ray._inverseDir[i];

		//Upper side intersect
		double t1 = (Max()[i] - ray._startPos[i]) * ray._inverseDir[i];

		//If the rays decening through box instead of ascending, switch t0 and t1 to keep following calcs correct
		if (ray._inverseDir[i] < 0.0f)
		{
			std::swap(t0, t1);
		}

		//Check t's lie between the part of the ray we're scrutinizing
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		if (tMax <= tMin)
		{
			return false;
		}
	}

	return true;
}
