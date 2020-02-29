#pragma once
#include "Utilities.h"
#include "Hittable.h"

class Material
{
public:
	Material();
	Material(const Material&) = delete;
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	bool const MaterialActive();

	virtual AA::Vec3 MaterialCalculatedColour(const AA::Ray& prevRay, const Hittable::HitResult& prevHit, Light* sceneLight);

	sf::Color GetColour();
	void SetColour(sf::Color col);

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};