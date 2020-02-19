#include "..\include\Light.h"

Light::Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos) : _statics(staticObjects), _dynamics(dynamicObjects), _position(pos)
{
}

Light::~Light()
{
}

void Light::CalculateLighting(const AA::Ray& ray, Hittable::HitResult& res)
{
	//Just does a check purely based on collisions to create a shadow ray
	Hittable::HitResult staticHit, dynamicHit;
	bool didHit = false;
	AA::Vec3 dirVec = ray.GetPointAlongRay(res.t) - _position;
	dirVec = AA::Vec3::UnitVector(dirVec);

	AA::Ray tempRay = AA::Ray(_position, dirVec);
	if (_statics != nullptr)
	{
		didHit = _statics->IntersectedRayOnly(tempRay, 0.0, INFINITY, staticHit);
	}

	if (_dynamics != nullptr)
	{
		didHit = _dynamics->IntersectedRayOnly(tempRay, 0.0, staticHit.t, dynamicHit) ? true : didHit;
	}


	//If it did intersect with summin then make it black
	if (didHit)
	{
		res.col = sf::Color(0, 0, 0, 255);
	}
}
