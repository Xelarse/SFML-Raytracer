#pragma once
#include "Hittable.h"


class Sphere : public Hittable
{
public:

	Sphere(AA::Vec3 o, double r, bool isStatic, sf::Color col, bool useColour, Light* sceneLight = nullptr);
	~Sphere() override;

	inline double SphereRadius() const { return _radius; };
	inline AA::Vec3 SphereOrigin() const { return _origin; };

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

private:
	AA::Vec3 _origin;
	double _radius;
};

