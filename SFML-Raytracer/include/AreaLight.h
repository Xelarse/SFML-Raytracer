#pragma once
#include "Light.h"
#include <random>

class AreaLight : public Light
{
public:
	AreaLight() = delete;
	AreaLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AA::Vec2 dims, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender);
	~AreaLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res, const bool& isRecursive = false) override;
	AA::Vec3 CalculateLightingForMaterial(const AA::Ray& inRay, const Hittable::HitResult& res) override;

private:
	int _samples = 10;
	AA::Vec2 _dims;
	AA::Vec3 _normal;
	std::mt19937 _ranGenerator;
};

