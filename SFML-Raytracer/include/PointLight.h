#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = delete;
	PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, bool debugRender);
	~PointLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res) override;
};

