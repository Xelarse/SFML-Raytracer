#include "..\include\Material.h"

Material::Material(sf::Color col, bool useMaterialProperties) : _colour(col), _useMaterial(useMaterialProperties)
{
}

Material::~Material()
{
}
