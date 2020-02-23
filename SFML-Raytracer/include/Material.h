#pragma once
#include "Utilities.h"

class Material
{
public:
	Material();
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	bool const MaterialActive();

	virtual AA::Vec3 MaterialCalculatedColour();
	virtual Material GetCopy();

	sf::Color GetColour();
	void SetColour(sf::Color col);

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};

