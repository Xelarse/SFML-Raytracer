#pragma once
#include "Material.h"

class Mirror : public Material
{
public:
	Mirror() = delete;
	Mirror(sf::Color col, bool useMaterialProperties, Hittable* statics, Hittable* dynamics);
	~Mirror() override;

	AA::Vec3 MaterialCalculatedColour(const AA::Ray& prevRay, const Hittable::HitResult& prevHit, Light* sceneLight) override;

private:
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
};

