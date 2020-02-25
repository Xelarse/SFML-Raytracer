#pragma once
#include "Material.h"
#include "Hittable.h"

class Mirror : public Material
{
public:
	Mirror() = delete;
	Mirror(sf::Color col, bool useMaterialProperties, Hittable* _statics, Hittable* _dynamics);
	~Mirror() override;

	AA::Vec3 MaterialCalculatedColour(AA::Vec3 rayStart, AA::Vec3 rayHit, AA::Vec3 surfaceNormal) override;

private:
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
};

