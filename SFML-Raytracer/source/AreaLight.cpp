#include "..\include\AreaLight.h"

AreaLight::AreaLight(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, AA::Vec2 dims, int sampleCount, sf::Color lightColour, double intensityMod, bool debugRender) : Light(staticObjects, dynamicObjects, pos, lightColour, intensityMod, debugRender), _samples(sampleCount), _dims(dims)
{
    //Set up the random generator for later use
    std::random_device rd;
    _ranGenerator = std::mt19937(rd());
}

AreaLight::~AreaLight()
{
}

void AreaLight::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res)
{
    Hittable::HitResult staticRes, dynamicRes, lightRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, collisionPoint);
    AA::Vec3 outCol = AA::Vec3(0, 0, 0);
    double boundsArea = _dims.X() * _dims.Y();

    //Calc the normal of the surface
    double xDimHalf = _dims.X() * 0.5;
    double yDimHalf = _dims.Y() * 0.5;
    AA::Vec3 x0y0 = _position - AA::Vec3(xDimHalf, yDimHalf, 0);
    AA::Vec3 x0y1 = _position + AA::Vec3(-xDimHalf, yDimHalf, 0);
    AA::Vec3 x1y0 = _position + AA::Vec3(xDimHalf, -yDimHalf, 0);

    AA::Vec3 v0v1 = x1y0 - x0y0;
    AA::Vec3 v0v2 = x0y1 - x0y0;

    _normal = v0v1.CrossProduct(v0v2).UnitVector();

    //Determine the boundaries in which the points can lie
    std::uniform_real_distribution<double> xDist(_position.X() - xDimHalf, _position.X() + xDimHalf);
    std::uniform_real_distribution<double> yDist(_position.Y() - yDimHalf, _position.Y() + yDimHalf);

    //Run as many rays at there are samples, average out the result
    for (int i = 0; i < _samples; ++i)
    {
        //Craft out random position that lies within the light bounds
        AA::Vec3 lightPosition = AA::Vec3(xDist(_ranGenerator), yDist(_ranGenerator), _position.Z());

        //Adjust outray to match the new position its sampled to and shift it slightly along its normal
        outRay._dir = AA::Vec3::UnitVector(lightPosition - collisionPoint);
        outRay._startPos = collisionPoint;
        outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

        //Do the material calc based on the new data from the new outRay
        AA::Vec3 materialCalc = res.mat->MaterialActive() ? res.mat->MaterialCalculatedColour(inRay._startPos, res.p, res.normal, outRay) : AA::Vec3(res.col.r / 255, res.col.g / 255, res.col.b / 255);

        //Check if the dot of the hit max zero returns zero and if it does the light calc doesnt need to be done as the normal is the opposide side to the light ray
        double nDotDHit = std::max(res.normal.DotProduct(outRay._dir), 0.0);
        if(nDotDHit == 0.0) { continue; }
        double nDotDLight = _normal.DotProduct(AA::Vec3::UnitVector(collisionPoint - lightPosition));

        //Find the t of this ray
        IntersectedRayOnly(outRay, 0, INFINITY, lightRes);

        //Check against a hit with both static and dynamics
        staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, lightRes.t, staticRes);
        dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, lightRes.t, dynamicRes);

        if (!staticHit && !dynamicHit)
        {
            AA::Vec3 reflectance = ((nDotDHit * nDotDLight) / (lightRes.t * lightRes.t) ) * materialCalc * _lightColorVec * _intensityMod;

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
