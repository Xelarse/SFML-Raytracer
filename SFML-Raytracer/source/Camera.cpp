#include "..\include\Camera.h"


Camera::Camera(AA::Vec3 lookFrom, AA::Vec3 lookAt, AA::Vec3 vUp, double vFov, double aspect)
	: _lookFrom(lookFrom), _lookAt(lookAt), _up(vUp), _vFov(vFov), _aspect(aspect)
{
	UpdateCameraInternals();
}

void Camera::SetLookFrom(AA::Vec3 newLookFrom)
{
	_lookFrom = newLookFrom;
	UpdateCameraInternals();
}

void Camera::SetLookAt(AA::Vec3 newLookAt)
{
	_lookAt = newLookAt;
	UpdateCameraInternals();
}

void Camera::SetVFov(double newVFov)
{
	_vFov = newVFov;
	UpdateCameraInternals();
}

AA::Ray Camera::GetRay(const double& s, const double& t)
{
	return AA::Ray(_lookFrom, _lowerLeftCorn + s*_horizontal + t*_vertical - _lookFrom);
}

void Camera::UpdateCameraInternals()
{
	double theta = _vFov * AA::PI / 180;
	double halfHeight = tan(theta / 2);
	double halfWidth = _aspect * halfHeight;
	AA::Vec3 u, v, w;

	w = AA::Vec3::UnitVector(_lookFrom - _lookAt);
	u = AA::Vec3::UnitVector(_up.CrossProduct(w));
	v = w.CrossProduct(u);

	_lowerLeftCorn = _lookFrom - halfWidth * u - halfHeight * v - w;
	_horizontal = 2 * halfWidth * u;
	_vertical = 2 * halfHeight * v;
}
