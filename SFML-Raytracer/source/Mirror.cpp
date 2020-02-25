#include "..\include\Mirror.h"

Mirror::Mirror(sf::Color col, bool useMaterialProperties, Hittable* _statics, Hittable* _dynamics)
{
}

Mirror::~Mirror()
{
}

AA::Vec3 Mirror::MaterialCalculatedColour(AA::Vec3 rayStart, AA::Vec3 rayHit, AA::Vec3 surfaceNormal)
{
	//Raycast out from the hit location towards where the initial ray started
	Hittable::HitResult staticRes, dynamicRes;
	bool staticHit, dynamicHit;
	AA::Ray outRay = AA::Ray(rayHit, surfaceNormal);
	outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

	staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, INFINITY, staticRes);
	dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, INFINITY, dynamicRes);

	//If it hits something get the colour information from the returned object and maybe diffuse it?
	if (staticHit && dynamicHit)
	{
		//Find the closest one AKA the lowest t and use that colour
		//TODO talk to tom about stopping the resursion here
		return staticRes.t < dynamicRes.t ? staticRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0)) : dynamicRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0));
	}
	else if (staticHit)
	{
		//return the colour information from the static hit
		return staticRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0));
	}
	else if (dynamicHit)
	{
		//Return the colour information from the dynamic hit
		return dynamicRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0));
	}
	else
	{
		//return the background colour
		return AA::BackgroundGradientCol((1.0 / surfaceNormal));
	}
}
