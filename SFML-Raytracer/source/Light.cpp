#include "..\include\Light.h"

Light::Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, bool debugRender) : _statics(staticObjects), _dynamics(dynamicObjects), _position(pos), _debugRender(debugRender)
{
    _col = sf::Color(212, 186, 42, 255);
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

bool Light::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _col;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _col;
            return true;
        }
    }

    return false;
}

bool Light::IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            return true;
        }
    }

    return false;
}

bool Light::BoundingBox(double t0, double t1, AABB& outBox) const
{
    outBox = AABB(
        _position - AA::Vec3(_sphereRadius, _sphereRadius, _sphereRadius),
        _position + AA::Vec3(_sphereRadius, _sphereRadius, _sphereRadius)
    );

    return true;
}

void Light::Move(AA::Vec3 newPos)
{
    if (_isStatic)
    {
        return;
    }
    _position = newPos;
}

void Light::Scale(AA::Vec3 newScale)
{
}
