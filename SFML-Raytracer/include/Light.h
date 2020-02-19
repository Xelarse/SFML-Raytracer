#pragma once
#include "Utilities.h"
#include "Hittable.h"

class Light
{
public:

	Light() = delete;
	Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos);
	virtual ~Light();

	virtual void CalculateLighting(const AA::Ray& ray, Hittable::HitResult& res);

	inline const AA::Vec3 GetPosition() { return _position; }
	inline void SetPosition(AA::Vec3 pos) { _position = pos; }

	inline const sf::Color GetLightColour() { return _lightColour; }
	inline void SetLightColour(sf::Color col) { _lightColour = col; }

private:

	AA::Vec3 _position;
	sf::Color _lightColour;
	Hittable* _statics = nullptr;
	Hittable* _dynamics = nullptr;
};

