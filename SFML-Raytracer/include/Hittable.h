#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities.h"
#include "AABB.h"

class Light;
class Material;

class Hittable
{
public:

	struct HitResult
	{
		double t;
		AA::Vec3 p;
		AA::Vec3 normal;
		sf::Color col;
		Material* mat;
	};

	Hittable();
	Hittable(bool isStatic, Material* mat, Light* sceneLight);
	virtual ~Hittable();

	//Override function for detecting if a ray has hit an object
	virtual bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) = 0;
	virtual bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) = 0;

	//Override function for Drawing a AABB around an object, Bool as some things might not have one like infinite planes and wont be included in the BVH
	//t0 and t1 used to ensure bounding box follows moving objects over a frame
	virtual bool BoundingBox(double t0, double t1, AABB& outBox) const = 0;

	//Basic functions for derviatives to implement for their respective object
	virtual void Move(AA::Vec3 newPos) = 0;
	virtual void Scale(AA::Vec3 newScale) = 0;

protected:
	bool _isStatic = false;
	std::unique_ptr<Material> _material;

	//Not owned just referenced
	Light* _sceneLight = nullptr;
};

