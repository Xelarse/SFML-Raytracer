#pragma once
#include "Utilities.h"

class Camera
{
public:

	Camera() = delete;
	Camera(const int& width, const int& height);
	~Camera() = default;

	AA::Vec3 GetDir(const int& x, const int& y, const int& width, const int& height);
	AA::Vec3 Origin() const;

private:

	AA::Vec3 _up = AA::Vec3(0, 2, 0);
	AA::Vec3 _right = AA::Vec3(4, 0, 0);
	AA::Vec3 _forward = AA::Vec3(0, 0, -2);
	AA::Vec3 _origin = AA::Vec3(0, 0, 0);
	AA::Vec3 _lowerLeft = AA::Vec3(-2.0, -1.0, -1.0);
};

