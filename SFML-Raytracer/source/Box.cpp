#include "..\include\Box.h"
#include <cmath>
#include <iostream>

Box::Box(AA::Vec3 origin, double width, double height, double depth, sf::Color col) : _origin(origin), _scale(AA::Vec3(width, height, depth))
{
    _col = col;
	UpdateBounds();
}

Box::~Box()
{
}

bool Box::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	double tXMin, tYMin, tZMin, tXMax, tYMax, tZMax;

    //Work out tmin and max for both x and y and sort them without having to swap based on which is bigger
    tXMin = (_bounds[ray._signs[0]].X() - ray._startPos.X()) * ray._inverseDir.X();
    tXMax = (_bounds[1 - ray._signs[0]].X() - ray._startPos.X()) * ray._inverseDir.X();
    tYMin = (_bounds[ray._signs[1]].Y() - ray._startPos.Y()) * ray._inverseDir.Y();
    tYMax = (_bounds[1 - ray._signs[1]].Y() - ray._startPos.Y()) * ray._inverseDir.Y();

    //Check if the ray hit lies within bounds and alligned on the x n y, if they are continue to z
    if ((tXMin > tYMax) || (tYMin > tXMax))
    {
        return false;
    }
    if (tYMin > tXMin)
    {
        tXMin = tYMin;
    }
    if (tYMax < tXMax)
    {
        tXMax = tYMax;
    }

    tZMin = (_bounds[ray._signs[2]].Z() - ray._startPos.Z()) * ray._inverseDir.Z();
    tZMax = (_bounds[1 - ray._signs[2]].Z() - ray._startPos.Z()) * ray._inverseDir.Z();

    if ((tXMin > tZMax) || (tZMin > tXMax))
    {
        return false;
    }
    if (tZMin > tXMin)
    {
        tXMin = tZMin;
    }
    if (tZMax < tXMax)
    {
        tXMax = tZMax;
    }

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

    if (_useColour)
    {
        res.col = _col;
    }
    else
    {
        res.col = AA::NormalToColour(res.normal);
    }

    return true;
}

void Box::MoveBox(AA::Vec3 pos)
{
	_origin = pos;
	UpdateBounds();
}

void Box::ScaleBox(double width, double height, double depth)
{
	_scale[0] = width;
	_scale[1] = height;
	_scale[2] = depth;
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
    //// for each of x y z check where it is in relation to the origin if its > origin, +1 if its < origin, -1
    //AA::Vec3 hitMinusOrigin = res.p - _origin;
    //AA::Vec3 absoluteHit = AA::Vec3(fabs(hitMinusOrigin._x), fabs(hitMinusOrigin._y), fabs(hitMinusOrigin._z));
    ////One of the axis should be the scale of box if its hit a face

    //if ((absoluteHit._x >= absoluteHit._y) && (absoluteHit._x >= absoluteHit._z))
    //{
    //    if (res.p._x < 0)
    //    {
    //        res.normal = AA::Vec3(-1, 0, 0);
    //    }
    //    else
    //    {
    //        res.normal = AA::Vec3(1, 0, 0);
    //    }
    //}
    //else if ((absoluteHit._y >= absoluteHit._x) && (absoluteHit._y >= absoluteHit._z))
    //{
    //    if (res.p._y < 0)
    //    {
    //        res.normal = AA::Vec3(0, -1, 0);
    //    }
    //    else
    //    {
    //        res.normal = AA::Vec3(0, 1, 0);
    //    }
    //}
    //else
    //{
    //    if (res.p._z < 0)
    //    {
    //        res.normal = AA::Vec3(0, 0, -1);
    //    }
    //    else
    //    {
    //        res.normal = AA::Vec3(0, 0, 1);
    //    }
    //}

    //Get vector from origin of cube to hit location use for Normal
    AA::Vec3 oToHit = res.p - _origin;
    res.normal = AA::Vec3::UnitVector(oToHit);
}

bool Box::BoundingBox(double t0, double t1, AABB& outBox) const
{
    outBox = AABB(
        _bounds[0],
        _bounds[1]
    );

    return true;
}
