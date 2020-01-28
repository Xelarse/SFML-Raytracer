#pragma once
#include "Hittable.h"

//TODO BVH calculations will most likely be done here in the near future
class Hittables
{
public:
	Hittables() = default;
	~Hittables() = default;

	bool IntersectedRay(const AA::Ray& ray, double tmin, double tmax, Hittable::HitResult& res);

	void AddHittable(std::unique_ptr<Hittable> hit);

private:

	std::vector<std::unique_ptr<Hittable>> _hittableObjects;
};

