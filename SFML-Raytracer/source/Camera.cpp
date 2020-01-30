#include "..\include\Camera.h"

Camera::Camera(const int& width, const int& height)
{
	int x = width / 100;
	int y = height / 100;

	_up._y = y * 2.0;
	_right._x = x * 2.0;

	_lowerLeft._x = -x;
	_lowerLeft._y = -y;
}

AA::Vec3 Camera::GetDir(const int& x, const int& y, const int& width, const int& height)
{
	float u = static_cast<float>(x) / static_cast<float>(width);
	float v = static_cast<float>(y) / static_cast<float>(height);

	return _lowerLeft + _right * u + _up * v;
}

AA::Vec3 Camera::Origin() const
{
	return _origin;
}
