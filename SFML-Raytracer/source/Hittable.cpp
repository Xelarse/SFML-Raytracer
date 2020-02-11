#include "..\include\Hittable.h"

Hittable::Hittable()
{
}

Hittable::Hittable(bool isStatic, sf::Color col, bool useColour) : _isStatic(isStatic), _col(col), _useColour(useColour)
{
}

Hittable::~Hittable()
{
}
