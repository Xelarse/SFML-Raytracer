#include "..\include\PointLight.h"

PointLight::PointLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender) : Light(staticObjects, dynamicObjects, pos, lightColour, intensityMod, debugRender)
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
    else
    {
        double nDotDRSquared = res.normal.DotProduct(outRay._dir) / (lightRes.t * lightRes.t);

        //Material colour properties, just default to the colour calc'd from the ray if its not using the material, most of the time its a colour based off the normal
        AA::Vec3 hitColourPid = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour() : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

        //Calc final colour
        AA::Vec3 finalColourVec = nDotDRSquared * hitColourPid * _lightColorVec * _intensityMod;

        //Clamp the values, this could be potentially messing with stuff
        finalColourVec[0] = finalColourVec[0] > 1.0 ? 1.0 : finalColourVec[0];
        finalColourVec[1] = finalColourVec[1] > 1.0 ? 1.0 : finalColourVec[1];
        finalColourVec[2] = finalColourVec[2] > 1.0 ? 1.0 : finalColourVec[2];

        res.col = finalColourVec.Vec3ToCol();
    }
}
