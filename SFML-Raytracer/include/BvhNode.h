#pragma once
#include "Utilities.h"
#include "Hittable.h"

//https://github.com/RayTracing/raytracing.github.io/tree/master/src/TheNextWeek Carry on here with BVH when i can


class BvhNode : public Hittable
{
public:
	BvhNode() = default;
	BvhNode(std::vector<Hittable*> dataPtr, double t0, double t1, bool useSmart);
	~BvhNode() = default;

	enum class AxisSort
	{
		X_AXIS = 0,
		Y_AXIS = 1,
		Z_AXIS = 2
	};

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;
	void ConstructBVH(std::vector<Hittable*> hittables, double t0, double t1, bool useSmart);

	void DumbConstruction(std::vector<Hittable*> hittables, double t0, double t1);
	void SmartConstruction(std::vector<Hittable*> hittables, double t0, double t1);

	void CalculateAxisCost(std::vector<Hittable*> hittables, AxisSort axis, double& outCost, int& outIndex);

	//Need to be implemented due to inheritance
	inline void Move(AA::Vec3 pos) override { return; }
	inline void Scale(AA::Vec3 scale) override { return; }

	//qsort comparison functions
	static int CompareXBox(Hittable* a, Hittable* b);
	static int CompareYBox(Hittable* a, Hittable* b);
	static int CompareZBox(Hittable* a, Hittable* b);

	inline bool IsConstructed() { return _left != nullptr && _right != nullptr; }


	//Nodes of the trees, could either lead down to move BvhNodes or stop at a Hittable
	Hittable* _left = nullptr;
	Hittable* _right = nullptr;
	AABB _box;
};

