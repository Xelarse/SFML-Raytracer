#pragma once
#include "Light.h"
#include <random>

class VolumeLight : public Light
{
public:
	VolumeLight() = delete;
	VolumeLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AABB boundary, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender);
	~VolumeLight() override;

	void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res, const bool& isRecursive = false) override;

private:
	int _samples = 10;
	AABB _boundary;
	std::mt19937 _ranGenerator;
};

