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
    //TODO fix this in a nice way to encompass depth, not iterations
    //if (isRecursive)
    //{
    //    if (_currentRecursion >= _recursionCap)
    //    {
    //        res.col = _shadowColour;
    //        return;
    //    }
    //    ++_currentRecursion;
    //}

    //Just does a check purely based on collisions to create a shadow ray
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Build a ray that fires from the point of collision to the light
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Check if the dot of the hit max zero returns zero and if it does the light calc doesnt need to be done as the normal is the opposide side to the light ray
    double nDotDHit = std::max(res.normal.DotProduct(outRay._dir), 0.0);
    if (nDotDHit == 0.0)
    {
        res.col = _shadowColour;
        return;
    }

    //Find the t of this ray
    IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

    //Check against a hit with both static and dynamics
    staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, lightRes.t, staticRes);
    dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, lightRes.t, dynamicRes);

    if (!staticHit && !dynamicHit || isRecursive)
    {
        //Material colour properties, just default to the colour calc'd from the ray if its not using the material, most of the time its a colour based off the normal
        AA::Vec3 materialCalc = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour(inRay, res, this) : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

        //Calc final colour
        AA::Vec3 finalColourVec = (nDotDHit /  (lightRes.t * lightRes.t)) * materialCalc * _lightColorVec * _intensityMod;

        //Tonemap using the selected method and set the colour
        res.col = AA::GammaTonemap(finalColourVec);
        return;
    }

    res.col = _shadowColour;
}
