#include "..\include\Hittables.h"

Hittables::Hittables(bool isHittableStatic, bool useBvh) : Hittable(isHittableStatic, sf::Color(255,255,255,255), false), _bvhEnabled(useBvh)
{
	_bvh = std::make_unique<BvhNode>();
}

Hittables::~Hittables()
{
	for (auto& obj : _hittableObjects)
	{
		delete obj;
	}
}

bool Hittables::IntersectedRay(const AA::Ray& ray, double tmin, double tmax, Hittable::HitResult& res)
{
	Hittable::HitResult tempRes;
	bool didHit = false;
	double closestHit = tmax;

	////With BVH
	//Create a bvh of the hittables, then do the iterations of the ray against the bvh intersect ray
	if (_bvhEnabled)
	{
		//If the objects can move then we need to remake the bvh, TODO Set a dirty flag later so this isnt done everyyyyy update
		if(!_isStatic || !_bvh->IsConstructed()) { ConstructBvh(); }
		didHit = _bvh->IntersectedRay(ray, tmin, tmax, tempRes);

		if (didHit)
		{
			res = tempRes;
		}
	}

	//// Without BVH
	else
	{
		for (auto& hitt : _hittableObjects)
		{
			if (hitt->IntersectedRay(ray, tmin, closestHit, tempRes))
			{
				didHit = true;
				closestHit = tempRes.t;
				res = tempRes;
			}
		}
	}

	return didHit;
}

//This function relies on the first object in the scene having a valid AABB, AKA FIRST OBJECT CANT BE AN INFITE PLANE
bool Hittables::BoundingBox(double t0, double t1, AABB& outBox) const
{
	if(_hittableObjects.size() < 1) { return false; }

	AABB tempBox;
	bool didExpand = false;
	
	if (!_hittableObjects.front()->BoundingBox(t0, t1, tempBox))
	{
		return false;
	}
	else
	{
		//Loop over all objects in the scene and expand the surrounding bounding box
		outBox = tempBox;
		for (int i = 1; i < _hittableObjects.size(); i++)
		{
			if (_hittableObjects[i]->BoundingBox(t0, t1, tempBox))
			{
				didExpand = true;
				outBox = AABB::SurroundingBox(outBox, tempBox);
			}
		}
	}

	//If the box never expanded just the box for the first object will do for later calcs, a full scene wide one isnt needed
	return didExpand;
}

void Hittables::ConstructBvh()
{
	_bvh->ConstructBVH(_hittableObjects, 0.0, 0.0, _isStatic);
}
