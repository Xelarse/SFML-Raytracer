#include "..\include\Diffuse.h"

Diffuse::Diffuse(sf::Color col, bool useMaterialProperties) : Material(col, useMaterialProperties)
{
}

Diffuse::~Diffuse()
{
}

AA::Vec3 Diffuse::MaterialCalculatedColour()
{
	//Do a basic diffuse
	AA::Vec3 col = AA::Vec3(_colour.r, _colour.g, _colour.b);
	col /= 255;
	return col / AA::PI;
}
