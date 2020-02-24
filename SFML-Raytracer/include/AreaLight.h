#pragma once
#include "Light.h"
#include <random>

class AreaLight : public Light
{
public:
	AreaLight() = delete;
	AreaLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AABB boundary, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender);
	~AreaLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res) override;

private:
	int _samples = 10;
	AABB _boundary;
	std::mt19937 _ranGenerator;
};

