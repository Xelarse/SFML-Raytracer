#include "..\include\Camera.h"


Camera::Camera(AA::Vec3 lookFrom, AA::Vec3 lookAt, AA::Vec3 vUp, double vFov, double aspect)
	: _origin(lookFrom), _lookAt(lookAt), _up(vUp), _vFov(vFov), _aspect(aspect)
{
	UpdateCameraInternals();
}

void Camera::SetLookFrom(AA::Vec3 newLookFrom)
{
	_origin = newLookFrom;
}

void Camera::SetLookAt(AA::Vec3 newLookAt)
{
	_lookAt = newLookAt;
}

AA::Ray Camera::GetRay(const double& s, const double& t)
{
	return AA::Ray(_origin, _lowerLeftCorn + s*_horizontal + t*_vertical - _origin);
}

AA::Vec3 Camera::RandInUnitDisk()
{
	AA::Vec3 p;
	do
	{
		p = 2.0 * AA::Vec3(AA::RanDouble(), AA::RanDouble(), 0) - AA::Vec3(1, 1, 0);
	} while (p.DotProduct(p) >= 1.0);
	return p;
}

void Camera::UpdateCameraInternals()
{
	double theta = _vFov * AA::PI / 180;
	double halfHeight = tan(theta / 2);
	double halfWidth = _aspect * halfHeight;
	AA::Vec3 u, v, w;

	w = AA::Vec3::UnitVector(_origin - _lookAt);
	u = AA::Vec3::UnitVector(_up.CrossProduct(w));
	v = w.CrossProduct(u);

	_lowerLeftCorn = _origin - halfWidth * u - halfHeight * v - w;
	_horizontal = 2 * halfWidth * u;
	_vertical = 2 * halfHeight * v;
}
