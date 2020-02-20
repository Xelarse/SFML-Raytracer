#include "..\include\Hittable.h"

Hittable::Hittable() : _material(Material(sf::Color(255, 0, 187,255), false))
{
}

Hittable::Hittable(bool isStatic, Material mat, Light* sceneLight) : _isStatic(isStatic), _material(mat), _sceneLight(sceneLight)
{
}

Hittable::~Hittable()
{
}
