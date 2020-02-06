#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <functional>
#include <random>
#include <vector>
#include <array>


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

		inline bool operator == (const Vec3& other) const
		{
			return _x == other._x && _y == other._y && _z == other._z;
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

	class Vec2
	{
	public:
		double _x, _y;
		Vec2() : _x(0), _y(0)
		{
		}
		Vec2(const double& x, const double& y) : _x(x), _y(y)
		{
		}

		//Operators
		inline Vec2 operator + (const Vec2& rh) const
		{
			return Vec2(_x + rh._x, _y + rh._y);
		}

		inline Vec2 operator - (const Vec2& rh) const
		{
			return Vec2(_x - rh._x, _y - rh._y);
		}

		inline Vec2 operator / (const Vec2& rh) const
		{
			return Vec2(_x / rh._x, _y / rh._y);
		}

		inline Vec2 operator / (const double& rh) const
		{
			return Vec2(_x / rh, _y / rh);
		}

		inline Vec2 operator * (const Vec2& rh) const
		{
			return Vec2(_x * rh._x, _y * rh._y);
		}

		inline Vec2 operator * (const double& rh) const
		{
			return Vec2(_x * rh, _y * rh);
		}

		inline Vec2& operator -= (const Vec2& rh)
		{
			_x -= rh._x;
			_y -= rh._y;
			return *this;
		}
		inline Vec2& operator += (const Vec2& rh)
		{
			_x += rh._x;
			_y += rh._y;
			return *this;
		}

		inline Vec2& operator += (const double& rh)
		{
			_x += rh;
			_y += rh;
			return *this;
		}

		inline Vec2& operator *= (const Vec2& rh)
		{
			_x *= rh._x;
			_y *= rh._y;
			return *this;
		}
		inline Vec2& operator *= (const double& rh)
		{
			_x *= rh;
			_y *= rh;
			return *this;
		}
		inline Vec2& operator /= (const Vec2& rh)
		{
			_x /= rh._x;
			_y /= rh._y;
			return *this;
		}
		inline Vec2& operator /= (const double& rh)
		{
			_x /= rh;
			_y /= rh;
			return *this;
		}

		inline bool operator == (const Vec2& other) const
		{
			return _x == other._x && _y == other._y;
		}

		//Vector math functions
		inline double DotProduct(const Vec2& b) const
		{
			return ((_x * b._x) + (_y * b._y));
		}

		inline double Length() const
		{
			return sqrt(_x * _x + _y * _y);
		}

		inline double SqrLength() const
		{
			return _x * _x + _y * _y;
		}

		inline Vec2 UnitVector() const
		{
			return *this / this->Length();
		}

		inline void MakeUnitVector()
		{
			double k = 1.0 / this->Length();
			_x *= k;
			_y *= k;
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
		~Ray() = default;
		inline Vec3 GetPointAlongRay(double t) const { return _startPos + _dir * t; }
	};

	class Vertex
	{
	public:
		Vertex() : _position(Vec3()), _normal(Vec3()), _texCord(Vec2()) { }
		Vertex(Vec3 pos, Vec3 norm, Vec2 tex) : _position(pos), _normal(norm), _texCord(tex) { }
		~Vertex() = default;
		Vec3 _position;
		Vec3 _normal;
		Vec2 _texCord;

		//Operators

		//used in unordered map to check for existance
		inline bool operator == (const Vertex& other) const
		{
			return _position == other._position && _normal == other._normal && _texCord == other._texCord;
		}
	};
	class Tri
	{
	public:
		Tri() = delete;
		Tri(std::array<Vertex, 3> verts) :  _verts(verts) {};
		~Tri() = default;
		std::array<Vertex, 3> _verts;
	};

	class Model
	{
	public:
		Model() = default;
		Model(std::vector<Tri> tris) : _tris( std::vector<Tri>(tris.begin(), tris.end() ) ) {}
		~Model() = default;
		std::vector<Tri> _tris;
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

	static AA::Vec3 LinearLerp(const AA::Vec3& a, const AA::Vec3& b, const double& t)
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

//Hash for Vec2
template<> struct std::hash<AA::Vec2>
{
	size_t operator()(AA::Vec2 const& vec) const
	{
		return ( (std::hash<double>()(vec._x) ^ (std::hash<double>()(vec._y) << 1)) );
	}
};

//Hash for Vec3
template<> struct std::hash<AA::Vec3>
{
	size_t operator()(AA::Vec3 const& vec) const
	{
		return ((std::hash<double>()(vec._x) ^ (std::hash<double>()(vec._y) << 1)) >> 1) ^ (std::hash<double>()(vec._z) << 1);
	}
};

//Hash for vertex
template<> struct std::hash<AA::Vertex>
{
	size_t operator()(AA::Vertex const& vertex) const
	{
		return ((std::hash<AA::Vec3>()(vertex._position) ^ (std::hash<AA::Vec3>()(vertex._normal) << 1)) >> 1) ^ (std::hash<AA::Vec2>()(vertex._texCord) << 1);
	}
};
