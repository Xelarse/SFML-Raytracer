#pragma once
#include "Material.h"

class Diffuse : public Material
{
public:
	Diffuse();
	Diffuse(sf::Color col, bool useMaterialProperties);
	~Diffuse() override;

	AA::Vec3 MaterialCalculatedColour(const AA::Ray& prevRay, const Hittable::HitResult& prevHit, Light* sceneLight) override;

private:
};

