#include "..\include\Material.h"

Material::Material(sf::Color col, bool useMaterialProperties) : _colour(col), _useMaterial(useMaterialProperties)
{
}

Material::~Material()
{
}

AA::Vec3 Material::MaterialCalculatedColour()
{
	//Do a basic diffuse
	return GetColourVecNormalised() / AA::PI;
}
