#pragma once
#include "Material.h"

class Diffuse : public Material
{
public:
	Diffuse();
	Diffuse(sf::Color col, bool useMaterialProperties);
	~Diffuse() override;

	AA::Vec3 MaterialCalculatedColour(AA::Vec3 rayStart, AA::Vec3 rayHit, AA::Vec3 surfaceNormal) override;

private:
};

