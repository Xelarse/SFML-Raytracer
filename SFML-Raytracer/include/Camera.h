#pragma once
#include "Utilities.h"

class Camera
{
public:

	Camera() = delete;
	Camera(AA::Vec3 lookFrom, AA::Vec3 lookAt, AA::Vec3 vUp, double vFov, double aspect);
	~Camera() = default;

	AA::Vec3 GetLookFrom() { return _lookFrom; }
	void SetLookFrom(AA::Vec3 newLookFrom);

	AA::Vec3 GetLookAt() { return _lookAt; }
	void SetLookAt(AA::Vec3 newLookAt);

	double GetVFov() { return _vFov; }
	void SetVFov(double newVFov);

	AA::Ray GetRay(const double& u, const double& v);

private:
	void UpdateCameraInternals();

	AA::Vec3 _lookFrom;
	AA::Vec3 _lowerLeftCorn;
	AA::Vec3 _horizontal;
	AA::Vec3 _vertical;


	AA::Vec3 _lookAt;
	AA::Vec3 _up;
	double _vFov;
	double _aspect;
};

