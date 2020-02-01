#include "..\include\Box.h"
#include <cmath>

Box::Box(AA::Vec3 origin, double width, double height, double depth) : _origin(origin), _scale(AA::Vec3(width, height, depth))
{
	UpdateBounds();
}

Box::~Box()
{
}

bool Box::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	double tXMin, tYMin, tZMin, tXMax, tYMax, tZMax;


    //Work out tmin and max for both x and y and sort them without having to swap based on which is bigger
    tXMin = (_bounds[ray._signs[0]]._x - ray._startPos._x) * ray._inverseDir._x;
    tXMax = (_bounds[1 - ray._signs[0]]._x - ray._startPos._x) * ray._inverseDir._x;
    tYMin = (_bounds[ray._signs[1]]._y - ray._startPos._y) * ray._inverseDir._y;
    tYMax = (_bounds[1 - ray._signs[1]]._y - ray._startPos._y) * ray._inverseDir._y;

    //Check if the ray hit lies within bounds and alligned on the x n y, if they are continue to z
    if ((tXMin > tYMax) || (tYMin > tXMax))
        return false;
    if (tYMin > tXMin)
        tXMin = tYMin;
    if (tYMax < tXMax)
        tXMax = tYMax;

    tZMin = (_bounds[ray._signs[2]]._z - ray._startPos._z) * ray._inverseDir._z;
    tZMax = (_bounds[1 - ray._signs[2]]._z - ray._startPos._z) * ray._inverseDir._z;

    if ((tXMin > tZMax) || (tZMin > tXMax))
        return false;
    if (tZMin > tXMin)
        tXMin = tZMin;
    if (tZMax < tXMax)
        tXMax = tZMax;

    res.t = tXMin;

    if (res.t < t_min)
    {
        res.t = tXMax;
        if (res.t < t_min)
        {
            return false;
        }
    }

    if (res.t > t_max)
    {
        return false;
    }

    res.p = ray.GetPointAlongRay(res.t);
    CalcNormal(res);
    return true;
}

void Box::MoveBox(AA::Vec3 pos)
{
	_origin = pos;
	UpdateBounds();
}

void Box::ScaleBox(double width, double height, double depth)
{
	_scale._x = width;
	_scale._y = height;
	_scale._z = depth;
	UpdateBounds();
}

void Box::UpdateBounds()
{
	AA::Vec3 deltaVec = _scale * 0.5;

	_bounds[0] = _origin - deltaVec;
	_bounds[1] = _origin + deltaVec;
}

void Box::CalcNormal(HitResult& res)
{
    // for each of x y z check where it is in relation to the origin if its > origin, +1 if its < origin, -1
    AA::Vec3 absoluteHit = AA::Vec3(abs(res.p._x), abs(res.p._y), abs(res.p._z));

    if (absoluteHit._x > absoluteHit._y)
    {
        absoluteHit._y = 0;
        
        if (absoluteHit._x > absoluteHit._z)
        {
            absoluteHit._z = 0;
            absoluteHit._x = res.p._x < 0 ? -1 : 1;
        }
        else
        {
            absoluteHit._x = 0;
            absoluteHit._z = res.p._z < 0 ? -1 : 1;
        }
    }
    else
    {
        absoluteHit._x = 0;

        if (absoluteHit._y > absoluteHit._z)
        {
            absoluteHit._z = 0;
            absoluteHit._y = res.p._y < 0 ? -1 : 1;
        }
        else
        {
            absoluteHit._y = 0;
            absoluteHit._z = res.p._z < 0 ? -1 : 1;
        }
    }

    res.normal = absoluteHit;
}
