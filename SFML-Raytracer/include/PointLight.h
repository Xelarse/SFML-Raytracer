#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = delete;
	PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender);
	~PointLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res, const bool& isRecursive = false) override;
	AA::Vec3 CalculateLightingForMaterial(const AA::Ray& inRay, const Hittable::HitResult& res) override;
};