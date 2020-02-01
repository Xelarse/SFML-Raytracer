#pragma once
#include "Hittable.h"

class Box : public Hittable
{
public:
	Box() = delete;
	Box(AA::Vec3 origin, double width, double height, double depth, sf::Color col);
	~Box();

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;

	void MoveBox(AA::Vec3 pos);
	void ScaleBox(double width, double height, double depth);

private:

	void UpdateBounds();
	void CalcNormal(HitResult& res);


	AA::Vec3 _scale;
	AA::Vec3 _origin;
	AA::Vec3 _bounds[2];
};

