#include "..\include\Hittables.h"

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
