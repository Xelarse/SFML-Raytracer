#pragma once
#include "Hittable.h"

class Box : public Hittable
{
public:
	Box() = delete;
	Box(AA::Vec3 origin, AA::Vec3 scale, bool isStatic, Material* mat, Light* sceneLight = nullptr);
	~Box() override;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

	AA::Vec3 GetPosition() { return _origin; }
	void OverrideNormal(AA::Vec3 norm);

private:

	void UpdateBounds();
	void CalcNormal(HitResult& res);

	bool _normalOverride = false;
	AA::Vec3 _overrideNormal;

	AA::Vec3 _scale;
	AA::Vec3 _origin;
	AA::Vec3 _bounds[2];
	std::array<AA::Vec3, 6> _faceNormals;
};

