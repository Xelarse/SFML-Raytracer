#include "..\include\Sphere.h"

Sphere::Sphere(AA::Vec3 o, double r, sf::Color col) : _origin(o), _radius(r)
{
	_col = col;
}

Sphere::~Sphere()
{
}

bool Sphere::IntersectedRay(const AA::Ray& ray)
{
	AA::Vec3 oc = ray._startPos - _origin;
	float a = ray._dir.DotProduct(ray._dir);
	float b = 2.0 * oc.DotProduct(ray._dir);
	float c = oc.DotProduct(oc) - _radius * _radius;

	float discrim = b * b - 4 * a * c;
	return discrim > 0;
}
