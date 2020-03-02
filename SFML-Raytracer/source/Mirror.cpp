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
	AA::Ray materialRay = AA::Ray(prevHit.p, prevHit.normal);
	materialRay._startPos = materialRay.GetPointAlongRay(AA::kEpsilon);

	staticHit = _statics == nullptr ? false : _statics->IntersectedRay(materialRay, 0.0, INFINITY, staticRes);
	dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRay(materialRay, 0.0, INFINITY, dynamicRes);

	AA::Vec3 resultantCol;

	//If it hits something get the colour information from the returned object and maybe diffuse it?
	if (staticHit && dynamicHit)
	{
		//Find the closest one AKA the lowest t and use that colour
		if (staticRes.t < dynamicRes.t)
		{
			resultantCol = sceneLight->CalculateLightingForMaterial(prevRay, staticRes);
		}
		else
		{
			resultantCol = sceneLight->CalculateLightingForMaterial(prevRay, dynamicRes);
		}
	}
	else if (staticHit)
	{
		resultantCol = sceneLight->CalculateLightingForMaterial(prevRay, staticRes);
	}
	else if (dynamicHit)
	{
		resultantCol = sceneLight->CalculateLightingForMaterial(prevRay, dynamicRes);
	}
	else
	{
		resultantCol = AA::BackgroundGradientCol(prevHit.normal);
	}

	//Add the mirror colour to the result and use the alpha channel as a strength for the tint
	return resultantCol + (AA::colToVec3(_colour) * AA::InverseLerp(0, 255, _colour.a));
}
