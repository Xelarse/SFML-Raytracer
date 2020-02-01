#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <functional>
#include <random>


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
			return sf::Color(_y * 255.0, _x * 255.0, _z * 255.0, 255.0);
		}
	};

	class Ray
	{
	public:
		Vec3 _startPos;
		Vec3 _dir;
		Vec3 _inverseDir;
		int _signs[3];
		Ray(const Vec3& startPos, const Vec3& dir) : _startPos(startPos), _dir(dir)
		{
			_inverseDir._x = 1 / dir._x;
			_inverseDir._y = 1 / dir._y;
			_inverseDir._z = 1 / dir._z;

			_signs[0] = (_inverseDir._x < 0);
			_signs[1] = (_inverseDir._y < 0);
			_signs[2] = (_inverseDir._z < 0);
		}
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

	static AA::Vec3 LinearLerp(const AA::Vec3& a, const AA::Vec3& b, const float& t)
	{
		AA::Vec3 lh(a._x * (1 - t), a._y * (1 - t), a._z * (1 - t));
		AA::Vec3 rh(b._x * t, b._y * t, b._z * t);
		return AA::Vec3(lh._x + rh._x, lh._y + rh._y, lh._z + rh._z);
	}

	//Returns a double inclusive 0, exclusive 1
	static double RanDouble()
	{
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		static std::function<double()> rand_generator =
			std::bind(distribution, generator);
		return rand_generator();
	}
}
