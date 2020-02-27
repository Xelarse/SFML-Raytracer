#pragma once
#include "Material.h"

class Diffuse : public Material
{
public:
	Diffuse();
	Diffuse(sf::Color col, bool useMaterialProperties);
	~Diffuse() override;

	AA::Vec3 MaterialCalculatedColour(const AA::Vec3& camRayStart, const AA::Vec3& camRayHit, const AA::Vec3& camRayNormal, const AA::Ray& outRay) override;

private:
};

