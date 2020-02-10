#pragma once
#include "Utilities.h"
#include "Hittable.h"

//https://github.com/RayTracing/raytracing.github.io/tree/master/src/TheNextWeek Carry on here with BVH when i can


class BvhNode : public Hittable
{
public:
	BvhNode() = default;
	BvhNode(std::vector<Hittable*> dataPtr, double t0, double t1);
	~BvhNode() = default;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;
	void ConstructBVH(std::vector<Hittable*> hittables, double t0, double t1);
	

	//qsort comparison functions
	static int CompareXBox(Hittable* a, Hittable* b);
	static int CompareYBox(Hittable* a, Hittable* b);
	static int CompareZBox(Hittable* a, Hittable* b);


	//Nodes of the trees, could either lead down to move BvhNodes or stop at a Hittable
	Hittable* _left = nullptr;
	Hittable* _right = nullptr;
	AABB _box;
};

