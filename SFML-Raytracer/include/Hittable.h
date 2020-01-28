#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities.h"

class Hittable
{
public:
	Hittable();
	virtual ~Hittable();

	virtual bool IntersectedRay(const AA::Ray& ray) = 0;

	inline sf::Color Colour() const { return _col; };

protected:
	sf::Color _col;
};

