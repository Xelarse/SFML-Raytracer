#include "..\include\Mirror.h"
#include "Light.h"

Mirror::Mirror(sf::Color col, bool useMaterialProperties, Hittable* statics, Hittable* dynamics) : Material(col, useMaterialProperties), _statics(statics), _dynamics(dynamics)
{
}

Mirror::~Mirror()
{
}

AA::Vec3 Mirror::MaterialCalculatedColour(const AA::Ray& prevRay, const Hittable::HitResult& prevHit, Light* sceneLight)
{
	//Raycast out from the hit location towards where the initial ray started
	Hittable::HitResult staticRes, dynamicRes;
	bool staticHit, dynamicHit;
	AA::Ray materialRay = AA::Ray(prevHit.p, AA::reflectRay(AA::Vec3::UnitVector(prevHit.p - sceneLight->GetPosition()), prevHit.normal));
	materialRay._startPos = materialRay.GetPointAlongRay(AA::kEpsilon);

	staticHit = _statics == nullptr ? false : _statics->IntersectedRay(materialRay, 0.0, INFINITY, staticRes);
	dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRay(materialRay, 0.0, INFINITY, dynamicRes);

	//If it hits something get the colour information from the returned object and maybe diffuse it?
	if (staticHit && dynamicHit)
	{
		//Find the closest one AKA the lowest t and use that colour
		if (staticRes.t < dynamicRes.t)
		{
			sceneLight->CalculateLighting(materialRay, staticRes, true);
			return AA::colToVec3(staticRes.col);
		}
		else
		{
			sceneLight->CalculateLighting(materialRay, dynamicRes, true);
			return AA::colToVec3(dynamicRes.col);
		}
	}
	else if (staticHit)
	{
		//return the colour information from the static hit
		sceneLight->CalculateLighting(materialRay, staticRes, true);
		return AA::colToVec3(staticRes.col);
	}
	else if (dynamicHit)
	{
		//Return the colour information from the dynamic hit
		sceneLight->CalculateLighting(materialRay, dynamicRes, true);
		return AA::colToVec3(dynamicRes.col);
	}
	else
	{
		//return the background colour
		return AA::BackgroundGradientCol((prevHit.normal));
	}
}
