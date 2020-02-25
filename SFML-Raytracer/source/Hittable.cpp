#include "..\include\Hittable.h"

Hittable::Hittable() : _material(new Material(sf::Color(0, 0, 0,255), false))
{
}

Hittable::Hittable(bool isStatic, Material* mat, Light* sceneLight) : _isStatic(isStatic), _material(std::move(mat)), _sceneLight(sceneLight)
{
}

Hittable::~Hittable()
{
}
