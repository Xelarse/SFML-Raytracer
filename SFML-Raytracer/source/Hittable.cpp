#include "..\include\Hittable.h"

Hittable::Hittable()
{
}

Hittable::Hittable(bool isStatic, sf::Color col, bool useColour, Light* sceneLight) : _isStatic(isStatic), _col(col), _useColour(useColour), _sceneLight(sceneLight)
{
}

Hittable::~Hittable()
{
}
