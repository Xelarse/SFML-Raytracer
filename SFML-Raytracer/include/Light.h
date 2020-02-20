#pragma once
#include "Utilities.h"
#include "Hittable.h"

class Light : public Hittable
{
public:

	Light() = delete;
	Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos);
	~Light() override;

	virtual void CalculateLighting(const AA::Ray& ray, Hittable::HitResult& res);

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;
	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;
	
	inline const AA::Vec3 GetPosition() { return _position; }

	inline const sf::Color GetLightColour() { return _col; }
	inline void SetLightColour(sf::Color col) { _col = col; }

private:

	AA::Vec3 _position;
	double _sphereRadius = 0.5;
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
};

