#pragma once
#include "Utilities.h"

class Material
{
public:
	Material() = delete;
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	bool const MaterialActive();

	virtual AA::Vec3 MaterialCalculatedColour();

	sf::Color GetColour();
	void SetColour(sf::Color col);

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};

