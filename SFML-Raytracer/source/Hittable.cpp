#include "..\include\Hittable.h"

Hittable::Hittable() : _col(sf::Color(0,0,0,255))
{
}

Hittable::Hittable(bool isStatic, sf::Color col, bool useColour, Light* sceneLight) : _isStatic(isStatic), _col(col), _useColour(useColour), _sceneLight(sceneLight)
{
}

Hittable::~Hittable()
{
}
