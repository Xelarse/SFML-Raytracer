#pragma once
#include "Utilities.h"

class Material
{
public:
	Material() = delete;
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	inline bool const MaterialActive() { return _useMaterial; }

	virtual AA::Vec3 MaterialCalculatedColour();

	inline sf::Color GetColour() { return _colour; }
	inline AA::Vec3 GetColourVec() { return AA::Vec3(_colour.r, _colour.g, _colour.b); }
	inline AA::Vec3 GetColourVecNormalised() { return AA::Vec3(_colour.r / 255, _colour.g / 255, _colour.b /255); }

	inline void SetColour(sf::Color col) { _colour = col; }

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};

