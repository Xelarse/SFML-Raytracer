#include "..\include\Hittables.h"

bool Hittables::IntersectedRay(const AA::Ray& ray, double tmin, double tmax, Hittable::HitResult& res)
{
	Hittable::HitResult tempRes;
	bool didHit = false;
	double closestHit = tmax;

	for (auto& hitt : _hittableObjects)
	{
		if (hitt->IntersectedRay(ray, tmin, closestHit, tempRes))
		{
			didHit = true;
			closestHit = tempRes.t;
			res = tempRes;
		}
	}

	return didHit;
}

void Hittables::AddHittable(std::unique_ptr<Hittable> hit)
{
	_hittableObjects.push_back(std::move(hit));
}
