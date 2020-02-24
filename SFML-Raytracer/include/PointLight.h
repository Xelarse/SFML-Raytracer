#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = delete;
	PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender);
	~PointLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res) override;
};