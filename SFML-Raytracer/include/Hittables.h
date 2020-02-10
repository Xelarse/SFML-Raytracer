#pragma once
#include "Hittable.h"

//TODO BVH calculations will most likely be done here in the near future
class Hittables : public Hittable
{
public:
	Hittables() = default;
	~Hittables();

	bool IntersectedRay(const AA::Ray& ray, double tmin, double tmax, Hittable::HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	bool _bvhEnabled;

	std::vector<Hittable*> _hittableObjects;
};

