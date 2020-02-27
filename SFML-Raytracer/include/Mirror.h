#pragma once
#include "Material.h"
#include "Hittable.h"

class Mirror : public Material
{
public:
	Mirror() = delete;
	Mirror(sf::Color col, bool useMaterialProperties, Hittable* statics, Hittable* dynamics);
	~Mirror() override;

	AA::Vec3 MaterialCalculatedColour(const AA::Vec3& camRayStart, const AA::Vec3& camRayHit, const AA::Vec3& camRayNormal, const AA::Ray& outRay) override;

private:
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
};

