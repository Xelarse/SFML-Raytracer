#pragma once
#include "Utilities.h"
#include "Hittable.h"
#include "Camera.h"

class Light : public Hittable
{
public:

	Light() = delete;
	Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender);
	~Light() override;

	virtual void CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res);

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;
	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;
	
	inline const AA::Vec3 GetPosition() { return _position; }

	inline const sf::Color GetLightColour() { return _lightColour; }
	inline const AA::Vec3 GetLightColourVec() { return _lightColorVec; }
	inline void SetLightColour(sf::Color col) {
		_lightColour = col;
		_lightColorVec = AA::Vec3(_lightColour.r / 255.0, _lightColour.g / 255.0, _lightColour.b / 255.0);
	}
	inline void SetLightColour(AA::Vec3 col)
	{
		_lightColour = sf::Color(col.R() * 255, col.G() * 255, col.B() * 255, 255);
		_lightColorVec = col;
	}

	inline const double GetIntensity() { return _intensityMod; }
	inline void SetIntensity(double newInten) { _intensityMod = newInten; }

	inline const bool IsDebugRendering() { return _debugRender; }
	inline void SetDebugRendering(bool active) { _debugRender = active; }

protected:

	AA::Vec3 _position;
	double _sphereRadius = 0.1;
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
	bool _debugRender = true;
	sf::Color _lightColour;
	AA::Vec3 _lightColorVec;
	double _intensityMod;
	const sf::Color _shadowColour = sf::Color(0, 0, 0, 255);
};

