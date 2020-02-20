#include "..\include\Light.h"

Light::Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, bool debugRender) 
    : _statics(staticObjects), _dynamics(dynamicObjects), _position(pos), _debugRender(debugRender)
{
    //_lightColour = sf::Color(212, 186, 42, 255);
    _lightColour = sf::Color(255, 255, 255, 255);
    _lightColorVec = AA::Vec3(_lightColour.r, _lightColour.g, _lightColour.b);
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

    //Build a ray that fires from the point of collision to the light
    AA::Vec3 collisionPoint = inRay.GetPointAlongRay(res.t);
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Find the t of this ray
    IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

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
            res.col = _lightColour;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _lightColour;
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
