#include "..\include\PointLight.h"

PointLight::PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, bool debugRender) : Light(staticObjects, dynamicObjects, pos, debugRender)
{
}

PointLight::~PointLight()
{
}

void PointLight::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res)
{
    //Just does a check purely based on collisions to create a shadow ray
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Build a ray that fires from the point of collision to the light
    AA::Vec3 collisionPoint = inRay.GetPointAlongRay(res.t);
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

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
    else
    {
        //For now just take the color from the material, later there will be functions related to doin a calc to get the right value out
        double nDotDRSquared = res.normal.DotProduct(outRay._dir) / (lightRes.t * lightRes.t);

        //Material colour properties
        AA::Vec3 hitColourPid = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour() : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

        //Calc final colour
        AA::Vec3 finalColourVec = nDotDRSquared * hitColourPid * _lightColorVec;
        res.col = finalColourVec.Vec3ToCol();
    }
}
