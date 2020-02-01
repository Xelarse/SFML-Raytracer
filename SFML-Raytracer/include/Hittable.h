#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities.h"

class Hittable
{
public:

	struct HitResult
	{
		double t;
		AA::Vec3 p;
		AA::Vec3 normal;
	};

	Hittable();
	virtual ~Hittable();

	virtual bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) = 0;

	inline sf::Color Colour() const { return _col; };

protected:
	sf::Color _col;
};

