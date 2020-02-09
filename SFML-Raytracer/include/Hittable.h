#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "AABB.h"

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

	//Override function for detecting if a ray has hit an object
	virtual bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) = 0;

	//Override function for Drawing a AABB around an object, Bool as some things might not have one like infinite planes and wont be included in the BVH
	//t0 and t1 used to ensure bounding box follows moving objects over a frame
	virtual bool BoundingBox(double t0, double t1, AABB& outBox) const = 0;

	inline sf::Color Colour() const { return _col; };
	inline bool UseColour() const { return _useColour; }
	inline void UseColour(bool use) { _useColour = use; }

protected:
	sf::Color _col;
	bool _useColour = false;
};

