#include "..\include\Light.h"

Light::Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, bool debugRender) 
    : _statics(staticObjects), _dynamics(dynamicObjects), _position(pos), _debugRender(debugRender), _rayCamera(Camera(pos, AA::Vec3(0,0,0), AA::Vec3(0,1,0), 80, (800 / 600) ))
{
    _col = sf::Color(212, 186, 42, 255);
}

Light::~Light()
{
}

void Light::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res)
{
	//Just does a check purely based on collisions to create a shadow ray
	Hittable::HitResult staticRes, dynamicRes, lightRes;
	bool staticHit = false;
    bool dynamicHit = false;

    ////Update Camera with new info and fire a ray
    //_rayCamera.SetLookFrom(_position);
    //_rayCamera.SetLookAt(inRay.GetPointAlongRay(res.t));
	//AA::Ray tempRay = _rayCamera.GetRay(1,1);

    //Build a ray that fires from the point of collision to the light
    AA::Vec3 collisionPoint = inRay.GetPointAlongRay(res.t);
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

    //Find the t of this ray
    IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

    //Send out both rays using the new ray with the new t_max
    if (_statics != nullptr)
    {
        staticHit = _statics->IntersectedRayOnly(outRay, 0.0, lightRes.t, staticRes);
    }

    if (_dynamics != nullptr)
    {
        dynamicHit = _dynamics->IntersectedRayOnly(outRay, 0.0, lightRes.t, dynamicRes);
    }

    if (staticHit || dynamicHit)
    {
        res.col = sf::Color(0, 0, 0, 255);
    }
}

bool Light::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _col;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _col;
            return true;
        }
    }

    return false;
}

bool Light::IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            return true;
        }
    }

    return false;
}

bool Light::BoundingBox(double t0, double t1, AABB& outBox) const
{
    return false;
}

void Light::Move(AA::Vec3 newPos)
{
    if (_isStatic)
    {
        return;
    }
    _position = newPos;
}

void Light::Scale(AA::Vec3 newScale)
{
}