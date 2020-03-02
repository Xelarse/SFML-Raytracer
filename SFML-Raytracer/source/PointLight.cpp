#include "..\include\PointLight.h"
#include "Material.h"

PointLight::PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender) : Light(staticObjects, dynamicObjects, pos, lightColour, intensityMod, debugRender)
{
}

PointLight::~PointLight()
{
}

void PointLight::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res, const bool& isRecursive)
{
    Hittable::HitResult staticRes, dynamicRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

    //Adjust outray to match the new position its sampled to and shift it slightly along its normal
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Do the material calc based on the new data from the new outRay
    AA::Vec3 materialCalc = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour(inRay, res, this) : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

    //Check if the dot of the hit max zero returns zero and if it does the light calc doesnt need to be done as the normal is the opposide side to the light ray
    double nDotDHit = std::max(res.normal.DotProduct(outRay._dir), 0.0);

    //Otherwise move onto the visibility check
    //Calc the distance from hit to light
    double dist = collisionPoint.Distance(_position);

    //Check against a hit with both static and dynamics
    staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, dist, staticRes);
    dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, dist, dynamicRes);

    if (!staticHit && !dynamicHit)
    {
        AA::Vec3 reflectance = (nDotDHit / (dist * dist)) * materialCalc * _lightColorVec * _intensityMod;

        //Tonemap using the selected method and set the colour
        res.col = AA::GammaTonemap(reflectance);
        return;
    }

    res.col = _shadowColour;
}

AA::Vec3 PointLight::CalculateLightingForMaterial(const AA::Ray& inRay, const Hittable::HitResult& res)
{
    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

    //Adjust outray to match the new position its sampled to and shift it slightly along its normal
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Do the material calc based on the new data from the new outRay
    AA::Vec3 materialCalc = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour(inRay, res, this) : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

    //Check if the dot of the hit max zero returns zero and if it does the light calc doesnt need to be done as the normal is the opposide side to the light ray
    double nDotDHit = std::max(res.normal.DotProduct(outRay._dir), 0.0);

    //Otherwise move onto the visibility check
    //Calc the distance from hit to light
    double dist = collisionPoint.Distance(_position);
    AA::Vec3 reflectance = (nDotDHit / (dist * dist)) * materialCalc * _lightColorVec * _intensityMod;

    //Tonemap using the selected method and set the colour
    return reflectance;
}
