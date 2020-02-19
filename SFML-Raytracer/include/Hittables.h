#pragma once
#include "Hittable.h"
#include "BvhNode.h"

class Hittables : public Hittable
{
public:
	Hittables() = delete;
	Hittables(bool isHittableStatic, bool useBvh, bool useSAH);
	~Hittables() override;

	bool IntersectedRay(const AA::Ray& ray, double tmin, double tmax, Hittable::HitResult& res) override;
	bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;
	void ConstructBvh();

	//Need to be implemented due to inheritance
	inline void Move(AA::Vec3 pos) override { return; }
	inline void Scale(AA::Vec3 scale) override { return; }

	std::vector<Hittable*> _hittableObjects;

private:
	bool _bvhEnabled = true;
	bool _sahEnabled = false;
	std::unique_ptr<BvhNode> _bvh;
};

