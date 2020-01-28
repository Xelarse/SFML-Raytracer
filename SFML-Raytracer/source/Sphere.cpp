#include "..\include\Sphere.h"

Sphere::Sphere(AA::Vec3 o, double r, sf::Color col) : _origin(o), _radius(r)
{
	_col = col;
}

Sphere::~Sphere()
{
}

bool Sphere::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) const
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
