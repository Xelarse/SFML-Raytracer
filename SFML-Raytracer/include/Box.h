#pragma once
#include "Hittable.h"

class Box : public Hittable
{
public:
	Box() = delete;
	Box(AA::Vec3 origin, AA::Vec3 scale, bool isStatic, sf::Color col, bool useColour);
	~Box() override;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

	AA::Vec3 GetPosition() { return _origin; }

private:

	void UpdateBounds();
	void CalcNormal(HitResult& res);


	AA::Vec3 _scale;
	AA::Vec3 _origin;
	AA::Vec3 _bounds[2];
};

