#include "..\include\VolumeLight.h"

VolumeLight::VolumeLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AABB boundary, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender) : Light(staticObjects, dynamicObjects, pos, lightColour, intensityMod, debugRender), _samples(sampleCount), _boundary(boundary)
{
    //Set up the random generator for later use
    std::random_device rd;
    _ranGenerator = std::mt19937(rd());
}

VolumeLight::~VolumeLight()
{
}

void VolumeLight::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res)
{
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Vec3 materialCalc = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour(inRay._startPos, res.p, res.normal) : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);
    AA::Ray outRay = AA::Ray(collisionPoint, collisionPoint);
    AA::Vec3 outCol = AA::Vec3(0, 0, 0);
    double boundsArea = std::abs(_boundary.Min().X() - _boundary.Max().X()) * std::abs(_boundary.Min().Y() - _boundary.Max().Y()) * std::abs(_boundary.Min().Z() - _boundary.Max().Z());

    //work out this updates bounds based off the AABB for scale + position
    std::array<AA::Vec3, 2> bounds;
    bounds[0] = _boundary.Min();
    bounds[1] = _boundary.Max();
    for (auto& b : bounds)
    {
        b[0] += _position[0];
        b[1] += _position[1];
        b[2] += _position[2];
    }

    std::uniform_real_distribution<double> xDist(bounds[0].X(), bounds[1].X());
    std::uniform_real_distribution<double> yDist(bounds[0].Y(), bounds[1].Y());
    std::uniform_real_distribution<double> zDist(bounds[0].Z(), bounds[1].Z());

    //Run as many rays at there are samples, average out the result
    for (int i = 0; i < _samples; ++i)
    {
        //Craft out random position that lies within the light bounds
        AA::Vec3 lightPosition = AA::Vec3(xDist(_ranGenerator), yDist(_ranGenerator), zDist(_ranGenerator));

        //Adjust outray to match the new position its sampled to and shift it slightly along its normal
        outRay._dir = AA::Vec3::UnitVector(lightPosition - collisionPoint);
        outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);   //TODO remove this from the recursive function or set it back to the point

        //Find the t of this ray
        IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

        //Check against a hit with both static and dynamics
        staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, lightRes.t, staticRes);
        dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, lightRes.t, dynamicRes);

        if (!staticHit && !dynamicHit)
        {
            //Do the fancy calc of the area light 
            double nDotDHit = res.normal.DotProduct(outRay._dir);
            double nDotDLight = lightRes.normal.DotProduct(collisionPoint - lightPosition); //TODO ask tom why using this makes the scene terribly dark, without it the spheres are just white

            //TODO move material calc into here for GGX, 
            //Calc value before the pdf(xi)
            //AA::Vec3 reflectance = ((nDotDHit * nDotDLight) / (lightRes.t * lightRes.t) )* materialCalc * _lightColorVec * _intensityMod;
            AA::Vec3 reflectance = (nDotDHit / (lightRes.t * lightRes.t)) * materialCalc * _lightColorVec * _intensityMod;

            //Safety check for div by zero
            if (reflectance == AA::Vec3(0, 0, 0) || reflectance == AA::Vec3(-0, -0, -0))
            {
                continue;
            }

            // Divide by PDF of sampling position on light source
            reflectance = reflectance / (1 / boundsArea);

            //Tonemap using the selected method and set the colour
            outCol += reflectance;
        }
    }

    //Average out the light based on the above taken samples and set it to the res col
    outCol /= _samples;
    res.col = outCol == AA::Vec3(0, 0, 0) || outCol.IsNAN() ? _shadowColour : AA::GammaTonemap(outCol);
}
