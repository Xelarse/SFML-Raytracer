#include "..\include\Material.h"

Material::Material() : _colour(sf::Color(232, 14, 188)), _useMaterial(true)
{
}

Material::Material(sf::Color col, bool useMaterialProperties) : _colour(col), _useMaterial(useMaterialProperties)
{
}

Material::~Material()
{
}

bool const Material::MaterialActive()
{
	return _useMaterial;
}

AA::Vec3 Material::MaterialCalculatedColour(const AA::Ray& prevRay, const Hittable::HitResult& prevHit, Light* sceneLight)
{
	//just return colour
	AA::Vec3 col = AA::Vec3(_colour.r, _colour.g, _colour.b);
	col /= 255;
	return col;
}

sf::Color Material::GetColour()
{
	return _colour;
}

AA::Vec3 Material::GetColourVec()
{
	AA::Vec3 col = AA::Vec3(_colour.r, _colour.g, _colour.b);
	col /= 255;
	return col;
}

void Material::SetColour(sf::Color col)
{
	_colour = col;
}
