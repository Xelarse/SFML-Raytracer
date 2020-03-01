#include "..\include\Box.h"
#include <cmath>
#include <iostream>
#include "Light.h"
#include "Material.h"

Box::Box(AA::Vec3 origin, AA::Vec3 scale, bool isStatic, Material* mat, Light* sceneLight)
    : Hittable(isStatic, mat, sceneLight), _origin(origin), _scale(scale)
{
    _faceNormals[0] = AA::Vec3(1, 0, 0);
    _faceNormals[1] = AA::Vec3(-1, 0, 0);
    _faceNormals[2] = AA::Vec3(0, 1, 0);
    _faceNormals[3] = AA::Vec3(0, -1, 0);
    _faceNormals[4] = AA::Vec3(0, 0, 1);
    _faceNormals[5] = AA::Vec3(0, 0, -1);

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

    if (_material->MaterialActive())
    {
        res.col = _material->GetColour();
    }
    else
    {
        res.col = AA::NormalToColour(res.normal);
        _material->SetColour(res.col);
    }
    res.mat = _material.get();

    if (_sceneLight != nullptr)
    {
        _sceneLight->CalculateLighting(ray, res);
    }

    return true;
}

void Box::Move(AA::Vec3 newPos)
{
    if(_isStatic) { return; }
	_origin = newPos;
	UpdateBounds();
}

void Box::Scale(AA::Vec3 newScale)
{
    if(_isStatic) { return; }
	_scale[0] = newScale.X();
	_scale[1] = newScale.Y();
	_scale[2] = newScale.Z();
	UpdateBounds();
}

void Box::OverrideNormal(AA::Vec3 norm)
{
    _overrideNormal = norm;
    _normalOverride = true;
}

void Box::UpdateBounds()
{
	AA::Vec3 deltaVec = _scale * 0.5;

	_bounds[0] = _origin - deltaVec;
	_bounds[1] = _origin + deltaVec;
}

void Box::CalcNormal(HitResult& res)
{
    if (_normalOverride)
    {
        res.normal = _overrideNormal;
        return;
    }

    const AA::Vec3 oToHit = AA::Vec3::UnitVector(res.p - _origin);
    float currentClosest = 0.0;
    int bestIndex = -1;

    for (int i = 0; i < _faceNormals.size(); ++i)
    {
        float val = oToHit.DotProduct(_faceNormals[i]);
        if (val > currentClosest&& val <= 1.0)
        {
            currentClosest = val;
            bestIndex = i;
        }
    }

    res.normal = _faceNormals[bestIndex];
}

bool Box::IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
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

    res.mat = _material.get();
    return true;
}

bool Box::BoundingBox(double t0, double t1, AABB& outBox) const
{
    outBox = AABB(
        _bounds[0],
        _bounds[1]
    );

    return true;
}
