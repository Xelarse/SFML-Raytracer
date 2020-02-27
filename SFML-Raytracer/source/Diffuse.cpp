#include "..\include\Diffuse.h"

Diffuse::Diffuse()
{
}

Diffuse::Diffuse(sf::Color col, bool useMaterialProperties) : Material(col, useMaterialProperties)
{
}

Diffuse::~Diffuse()
{
}

AA::Vec3 Diffuse::MaterialCalculatedColour(const AA::Vec3& camRayStart, const AA::Vec3& camRayHit, const AA::Vec3& camRayNormal, const AA::Ray& outRay)
{
	//Do a basic diffuse
	AA::Vec3 col = AA::Vec3(_colour.r, _colour.g, _colour.b);
	col /= 255;
	return col / AA::PI;
}
