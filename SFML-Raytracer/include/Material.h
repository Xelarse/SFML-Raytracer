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

	virtual AA::Vec3 MaterialCalculatedColour(AA::Vec3 rayStart, AA::Vec3 rayHit, AA::Vec3 surfaceNormal);

	sf::Color GetColour();
	void SetColour(sf::Color col);

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};