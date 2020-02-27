#include "..\include\Mirror.h"

Mirror::Mirror(sf::Color col, bool useMaterialProperties, Hittable* statics, Hittable* dynamics) : Material(col, useMaterialProperties), _statics(statics), _dynamics(dynamics)
{
}

Mirror::~Mirror()
{
}

AA::Vec3 Mirror::MaterialCalculatedColour(const AA::Vec3& camRayStart, const AA::Vec3& camRayHit, const AA::Vec3& camRayNormal, const AA::Ray& outRay)
{
	//Raycast out from the hit location towards where the initial ray started
	Hittable::HitResult staticRes, dynamicRes;
	bool staticHit, dynamicHit;
	AA::Ray materialRay = AA::Ray(camRayHit, camRayNormal);
	materialRay._startPos = materialRay.GetPointAlongRay(AA::kEpsilon);

	staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(materialRay, 0.0, INFINITY, staticRes);
	dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(materialRay, 0.0, INFINITY, dynamicRes);

	//If it hits something get the colour information from the returned object and maybe diffuse it?
	if (staticHit && dynamicHit)
	{
		//Find the closest one AKA the lowest t and use that colour
		//TODO talk to tom about stopping the resursion here
		return staticRes.t < dynamicRes.t ? 
			staticRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), outRay) : 
			dynamicRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), outRay);
	}
	else if (staticHit)
	{
		//return the colour information from the static hit
		return staticRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), outRay);
	}
	else if (dynamicHit)
	{
		//Return the colour information from the dynamic hit
		return dynamicRes.mat->MaterialCalculatedColour(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), AA::Vec3(0, 0, 0), outRay);
	}
	else
	{
		//return the background colour
		return AA::BackgroundGradientCol((camRayNormal));
	}
}
