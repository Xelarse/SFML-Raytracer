#include "..\include\AreaLight.h"

AreaLight::AreaLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AABB boundary, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender) : Light(staticObjects, dynamicObjects, pos, lightColour, intensityMod, debugRender), _samples(sampleCount), _boundary(boundary)
{
}

AreaLight::~AreaLight()
{
}

void AreaLight::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res)
{
    //Just does a check purely based on collisions to create a shadow ray
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    bool staticHit = false;
    bool dynamicHit = false;
    AA::Vec3 collisionPoint = inRay.GetPointAlongRay(res.t);
    AA::Ray outRay = AA::Ray(collisionPoint, collisionPoint);
    AA::Vec3 outCol = AA::Vec3(0, 0, 0);
    AA::Vec3 materialCalc = res.mat.MaterialActive() ? res.mat.MaterialCalculatedColour() : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

    for (int i = 0; i < _samples; ++i)
    {
        //Adjust outray to match the new position its sampled to and shift it slightly along its normal
        outRay._dir = AA::Vec3::UnitVector(newRandomPosInBoundary - collisionPoint);
        outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

        //Find the t of this ray
        IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

        //Check against a hit with both static and dynamics
        staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, lightRes.t, staticRes);
        dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, lightRes.t, dynamicRes);

        if (!staticHit && !dynamicHit)
        {
            //Do the fancy calc of the area light 
            double nDotDRSquared = res.normal.DotProduct(outRay._dir) * THE OTHER SHIT / (lightRes.t * lightRes.t);

            //Calc final colour
            AA::Vec3 finalColourVec = nDotDRSquared * materialCalc * _lightColorVec * _intensityMod;

            //Tonemap using the selected method and set the colour
            outCol += AA::GammaTonemapHDR(finalColourVec);
        }
    }


    //Average out the light based on the above taken samples and set it to the res col
    outCol /= _samples;
    res.col = outCol.Vec3ToCol();
}
