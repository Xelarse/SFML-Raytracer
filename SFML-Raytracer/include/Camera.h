#pragma once
#include "Utilities.h"

class Camera
{
public:

	Camera() = delete;
	Camera(AA::Vec3 lookFrom, AA::Vec3 lookAt, AA::Vec3 vUp, double vFov, double aspect);
	~Camera() = default;


	AA::Ray GetRay(const double& u, const double& v);

private:

	AA::Vec3 RandInUnitDisk();

	AA::Vec3 _origin;
	AA::Vec3 _lowerLeftCorn;
	AA::Vec3 _horizontal;
	AA::Vec3 _vertical;
};

