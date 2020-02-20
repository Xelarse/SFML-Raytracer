#pragma once
#include "Utilities.h"

class Material
{
public:
	Material() = delete;
	Material(sf::Color col, bool useMaterialProperties);
	virtual ~Material();

	inline sf::Color GetColour() { return _colour; }
	inline AA::Vec3 GetColourVec() { return AA::Vec3(_colour.r, _colour.g, _colour.b); }
	inline AA::Vec3 GetColourVecNormalised() { return AA::Vec3(_colour.r, _colour.g, _colour.b).UnitVector(); }

	inline void SetColour(sf::Color col) { _colour = col; }
	inline void SetColour(AA::Vec3 col)
	{
		_colour.r = col.R();
		_colour.g = col.G();
		_colour.b = col.B();
	}

protected:
	bool _useMaterial = false;
	sf::Color _colour;
};

