#include "..\include\Hittable.h"

Hittable::Hittable()
{
}

Hittable::Hittable(Light* sceneLight, bool isStatic, sf::Color col, bool useColour) : _isStatic(isStatic), _col(col), _useColour(useColour), _sceneLight(sceneLight)
{
}

Hittable::~Hittable()
{
}
