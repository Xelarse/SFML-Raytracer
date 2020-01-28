#pragma once
#include "Hittable.h"


class Sphere : public Hittable
{
public:

	Sphere(AA::Vec3 o, double r, sf::Color col);
	~Sphere() override;

	inline double SphereRadius() const { return _radius; };
	inline AA::Vec3 SphereOrigin() const { return _origin; };

	bool IntersectedRay(const AA::Ray& ray) override;

private:
	AA::Vec3 _origin;
	double _radius;
};

