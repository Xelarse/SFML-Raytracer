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
		sf::Color col;
	};

	Hittable();
	virtual ~Hittable();

	virtual bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) = 0;

	inline sf::Color Colour() const { return _col; };
	inline bool UseColour() const { return _useColour; }
	inline void UseColour(bool use) { _useColour = use; }

protected:
	sf::Color _col;
	bool _useColour = false;
};

