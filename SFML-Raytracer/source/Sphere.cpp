#include "..\include\Sphere.h"
#include "Light.h"

Sphere::Sphere(AA::Vec3 o, double r, bool isStatic, Material mat, Light* sceneLight) : Hittable(isStatic, mat, sceneLight), _origin(o), _radius(r)
{
}

Sphere::~Sphere()
{
}

bool Sphere::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	AA::Vec3 oc = ray._startPos - _origin;
	double a = ray._dir.DotProduct(ray._dir);
	double b = oc.DotProduct(ray._dir);
	double c = oc.DotProduct(oc) - _radius * _radius;

    double discrim = b * b - a * c;
	if (discrim > 0)
	{
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _origin) / _radius;
            if (_material.MaterialActive())
            {
                res.col = _material.GetColour();
            }
            else
            {
                res.col = AA::NormalToColour(res.normal);
                _material.SetColour(res.col);
            }
            res.mat = &_material;
            if (_sceneLight != nullptr)
            {
                _sceneLight->CalculateLighting(ray, res);
            }
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _origin) / _radius;
            if (_material.MaterialActive())
            {
                res.col = _material.GetColour();
            }
            else
            {
                res.col = AA::NormalToColour(res.normal);
                _material.SetColour(res.col);
            }
            res.mat = &_material;
            if (_sceneLight != nullptr)
            {
                _sceneLight->CalculateLighting(ray, res);
            }
            return true;
        }
	}

	return false;
}

bool Sphere::IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _origin;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _radius * _radius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _origin) / _radius;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _origin) / _radius;
            return true;
        }
    }

    return false;
}

bool Sphere::BoundingBox(double t0, double t1, AABB& outBox) const
{
    outBox = AABB(
        _origin - AA::Vec3(_radius, _radius, _radius),
        _origin + AA::Vec3(_radius, _radius, _radius)
    );

    return true;
}

void Sphere::Move(AA::Vec3 newPos)
{
    if(_isStatic) { return; }
    _origin = newPos;
}

void Sphere::Scale(AA::Vec3 newScale)
{
    if(_isStatic) { return; }
    _radius = newScale.X();
}
