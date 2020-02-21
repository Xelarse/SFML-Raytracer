#pragma once
#include "Material.h"

class Diffuse : public Material
{
public:
	Diffuse() = delete;
	Diffuse(sf::Color col, bool useMaterialProperties);
	~Diffuse() override;

	AA::Vec3 MaterialCalculatedColour() override;

private:
};

