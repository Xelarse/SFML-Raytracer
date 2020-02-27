#pragma once
#include "Utilities.h"

class Material
{
public:
	Material();
	Material(const Material&) = delete;
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	bool const MaterialActive();

	virtual AA::Vec3 MaterialCalculatedColour(const AA::Vec3& camRayStart, const AA::Vec3& camRayHit, const AA::Vec3& camRayNormal, const AA::Ray& outRay);

	sf::Color GetColour();
	void SetColour(sf::Color col);

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};