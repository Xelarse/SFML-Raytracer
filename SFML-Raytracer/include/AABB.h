#pragma once
#include "Utilities.h"


class AABB
{
public:
	AABB() = default;
	AABB(const AA::Vec3& min, const AA::Vec3& max) : _min(min), _max(max) { }


private:
	AA::Vec3 _min;
	AA::Vec3 _max;
};

