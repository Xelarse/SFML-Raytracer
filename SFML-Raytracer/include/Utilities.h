#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>


namespace AA
{
	class Vec3
	{
	public:
		double _x, _y, _z;
		Vec3() : _x(0), _y(0), _z(0) { }
		Vec3(const double& x, const double& y, const double& z) : _x(x), _y(y), _z(z) { }

		//Operators
		inline Vec3 operator + (const Vec3& rh) const
		{
			return Vec3(_x + rh._x, _y + rh._y, _z + rh._z);
		}

		inline Vec3 operator - (const Vec3& rh) const
		{
			return Vec3(_x - rh._x, _y - rh._y, _z - rh._z);
		}

		inline Vec3 operator / (const Vec3& rh) const
		{
			return Vec3(_x / rh._x, _y / rh._y, _z / rh._z);
		}

		inline Vec3 operator / (const double& rh) const
		{
			return Vec3(_x / rh, _y / rh, _z / rh);
		}

		inline Vec3 operator * (const Vec3& rh) const
		{
			return Vec3(_x * rh._x, _y * rh._y, _z * rh._z);
		}

		inline Vec3 operator * (const double& rh) const
		{
			return Vec3(_x * rh, _y * rh, _z * rh);
		}

		inline Vec3& operator -= (const Vec3& rh)
		{
			_x -= rh._x;
			_y -= rh._y; 
			_z -= rh._z;
			return *this;
		}
		inline Vec3& operator += (const Vec3& rh)
		{
			_x += rh._x; 
			_y += rh._y; 
			_z += rh._z;
			return *this;
		}

		inline Vec3& operator += (const double& rh)
		{
			_x += rh;
			_y += rh;
			_z += rh;
			return *this;
		}

		inline Vec3& operator *= (const Vec3& rh)
		{
			_x *= rh._x;
			_y *= rh._y;
			_z *= rh._z;
			return *this;
		}
		inline Vec3& operator *= (const double& rh)
		{
			_x *= rh;
			_y *= rh;
			_z *= rh;
			return *this;
		}
		inline Vec3& operator /= (const Vec3& rh)
		{
			_x /= rh._x;
			_y /= rh._y;
			_z /= rh._z;
			return *this;
		}
		inline Vec3& operator /= (const double& rh)
		{
			_x /= rh;
			_y /= rh;
			_z /= rh;
			return *this;
		}

		//Vector math functions
		inline double DotProduct(const Vec3& b) const
		{
			return ((_x * b._x) + (_y * b._y) + (_z * b._z));
		}

		inline Vec3 CrossProduct(const Vec3& b) const
		{
			return Vec3(
				_y * b._z - _z * b._y,
				_z * b._x - _x * b._z,
				_x * b._y - _y * b._x
			);
		}

		inline double Length() const
		{
			return sqrt(_x * _x + _y * _y + _z * _z);
		}

		inline double SqrLength() const
		{
			return _x * _x + _y * _y + _z * _z;
		}

		inline Vec3 UnitVector() const
		{
			return *this / this->Length();
		}

		inline void MakeUnitVector()
		{
			double k = 1.0 / this->Length();
			_x *= k;
			_y *= k;
			_z *= k;
		}

		inline sf::Color Vec3ToCol() const
		{
			return sf::Color(_x * 255, _y * 255, _z * 255, 255);
		}
	};

	class Ray
	{
	public:
		Vec3 _startPos;
		Vec3 _dir;
		Ray(const Vec3& startPos, const Vec3& dir) : _startPos(startPos), _dir(dir) { }
		inline Vec3 GetPointAlongRay(double t) const { return _startPos + _dir * t; }
	};

	class ColourArray
	{
	public:
		ColourArray() = delete;
		ColourArray(int columns, int rows) : _rows(rows), _columns(columns), _colours(std::vector<sf::Color>(rows * columns, sf::Color(0,0,0,255))) { }

		sf::Color& GetColourAtPosition(int& x, int& y)
		{
			return _colours.at(y * _columns + x);
		}

		void ColourPixelAtPosition(int& x, int& y, sf::Color& col)
		{
			_colours.at(y * _columns + x) = col;
		}

		void* GetDataBasePointer()
		{
			return reinterpret_cast<void*>(_colours.data());
		}

		size_t GetDataSize()
		{
			return _colours.size() * sizeof(sf::Color);
		}

	private:
		int _rows;
		int _columns;

		std::vector<sf::Color> _colours;
	};

	static sf::Color LinearLerp(const sf::Color& a, const sf::Color& b, const float& t)
	{
		sf::Color lh(a.r * (1 - t), a.g * (1 - t), a.b * (1 - t), 255);
		sf::Color rh(b.r * t, b.g * t, b.b * t, 255);
		return sf::Color(lh.r + rh.r, lh.g + rh.g, lh.b + rh.b, 255);
	}
}
