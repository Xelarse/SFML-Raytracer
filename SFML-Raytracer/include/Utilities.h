#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <functional>
#include <random>
#include <vector>
#include <array>


namespace AA
{
	static const double PI = 3.14159265358979323846;
	static const float kEpsilon = 1e-8;

	class Vec3
	{
	public:
		Vec3()
		{
			_e[0] = 0;
			_e[1] = 0;
			_e[2] = 0;
		}
		Vec3(const double& x, const double& y, const double& z) 
		{
			_e[0] = x;
			_e[1] = y;
			_e[2] = z;
		}

		//Accessors
		inline double X() const { return _e[0]; }
		inline double Y() const { return _e[1]; }
		inline double Z() const { return _e[2]; }
		inline double R() const { return _e[0]; }
		inline double G() const { return _e[1]; }
		inline double B() const { return _e[2]; }
		inline double operator[](int i) const { return _e[i]; }
		inline double& operator[](int i) { return _e[i]; }


		//Operators
		inline Vec3 operator + (const Vec3& rh) const
		{
			return Vec3(_e[0] + rh.X(), _e[1] + rh.Y(), _e[2] + rh.Z());
		}

		inline Vec3 operator - (const Vec3& rh) const
		{
			return Vec3(_e[0] - rh.X(), _e[1] - rh.Y(), _e[2] - rh.Z());
		}

		inline Vec3 operator / (const Vec3& rh) const
		{
			return Vec3(_e[0] / rh.X(), _e[1] / rh.Y(), _e[2] / rh.Z());
		}

		inline Vec3 operator / (const double& rh) const
		{
			return Vec3(_e[0] / rh, _e[1] / rh, _e[2] / rh);
		}

		inline Vec3 operator * (const Vec3& rh) const
		{
			return Vec3(_e[0] * rh.X(), _e[1] * rh.Y(), _e[2] * rh.Z());
		}

		inline Vec3 operator * (const double& rh) const
		{
			return Vec3(_e[0] * rh, _e[1] * rh, _e[2] * rh);
		}

		inline Vec3& operator -= (const Vec3& rh)
		{
			_e[0] -= rh.X();
			_e[1] -= rh.Y();
			_e[2] -= rh.Z();
			return *this;
		}
		inline Vec3& operator += (const Vec3& rh)
		{
			_e[0] += rh.X();
			_e[1] += rh.Y();
			_e[2] += rh.Z();
			return *this;
		}

		inline Vec3& operator += (const double& rh)
		{
			_e[0] += rh;
			_e[1] += rh;
			_e[2] += rh;
			return *this;
		}

		inline Vec3& operator *= (const Vec3& rh)
		{
			_e[0] *= rh.X();
			_e[1] *= rh.Y();
			_e[2] *= rh.Z();
			return *this;
		}
		inline Vec3& operator *= (const double& rh)
		{
			_e[0] *= rh;
			_e[1] *= rh;
			_e[2] *= rh;
			return *this;
		}
		inline Vec3& operator /= (const Vec3& rh)
		{
			_e[0] /= rh.X();
			_e[1] /= rh.Y();
			_e[2] /= rh.Z();
			return *this;
		}
		inline Vec3& operator /= (const double& rh)
		{
			_e[0] /= rh;
			_e[1] /= rh;
			_e[2] /= rh;
			return *this;
		}

		inline bool operator == (const Vec3& other) const
		{
			return _e[0] == other.X() && _e[1] == other.Y() && _e[2] == other.Z();
		}

		inline bool operator != (const Vec3& other) const
		{
			return _e[0] != other.X() && _e[1] != other.Y() && _e[2] != other.Z();
		}

		//Vector math functions
		inline double DotProduct(const Vec3& b) const
		{
			return ((_e[0] * b.X()) + (_e[1] * b.Y()) + (_e[2] * b.Z()));
		}

		inline Vec3 CrossProduct(const Vec3& b) const
		{
			return Vec3(
				_e[1] * b.Z() - _e[2] * b.Y(),
				_e[2] * b.X() - _e[0] * b.Z(),
				_e[0] * b.Y() - _e[1] * b.X()
			);
		}

		inline double Length() const
		{
			return sqrt(_e[0] * _e[0] + _e[1] * _e[1] + _e[2] * _e[2]);
		}

		inline double SqrLength() const
		{
			return _e[0] * _e[0] + _e[1] * _e[1] + _e[2] * _e[2];
		}

		inline Vec3 UnitVector() const
		{
			return *this / this->Length();
		}

		static inline Vec3 UnitVector(Vec3 v)
		{
			return v / v.Length();
		}

		inline void MakeUnitVector()
		{
			double k = 1.0 / this->Length();
			_e[0] *= k;
			_e[1] *= k;
			_e[2] *= k;
		}

		inline sf::Color Vec3ToCol() const
		{
			return sf::Color(_e[0] * 255.0, _e[1] * 255.0, _e[2] * 255.0, 255.0);
		}

	private:
		double _e[3];
	};

	inline Vec3 operator * (const double& lh, const Vec3& rh)
	{
		return Vec3(lh * rh.X(), lh * rh.Y(), lh * rh.Z());
	}

	class Vec2
	{
	public:
		Vec2()
		{
			_e[0] = 0;
			_e[1] = 0;
		}
		Vec2(const double& x, const double& y)
		{
			_e[0] = x;
			_e[1] = y;
		}

		//Accessors
		inline double X() const { return _e[0]; }
		inline double Y() const { return _e[1]; }
		inline double operator[](int i) const { return _e[i]; }
		inline double& operator[](int i) { return _e[i]; }

		//Operators
		inline Vec2 operator + (const Vec2& rh) const
		{
			return Vec2(_e[0] + rh.X(), _e[1] + rh.Y());
		}

		inline Vec2 operator - (const Vec2& rh) const
		{
			return Vec2(_e[0] - rh.X(), _e[1] - rh.Y());
		}

		inline Vec2 operator / (const Vec2& rh) const
		{
			return Vec2(_e[0] / rh.X(), _e[1] / rh.Y());
		}

		inline Vec2 operator / (const double& rh) const
		{
			return Vec2(_e[0] / rh, _e[1] / rh);
		}

		inline Vec2 operator * (const Vec2& rh) const
		{
			return Vec2(_e[0] * rh.X(), _e[1] * rh.Y());
		}

		inline Vec2 operator * (const double& rh) const
		{
			return Vec2(_e[0] * rh, _e[1] * rh);
		}

		inline Vec2& operator -= (const Vec2& rh)
		{
			_e[0] -= rh.X();
			_e[1] -= rh.Y();
			return *this;
		}
		inline Vec2& operator += (const Vec2& rh)
		{
			_e[0] += rh.X();
			_e[1] += rh.Y();
			return *this;
		}

		inline Vec2& operator += (const double& rh)
		{
			_e[0] += rh;
			_e[1] += rh;
			return *this;
		}

		inline Vec2& operator *= (const Vec2& rh)
		{
			_e[0] *= rh.X();
			_e[1] *= rh.Y();
			return *this;
		}
		inline Vec2& operator *= (const double& rh)
		{
			_e[0] *= rh;
			_e[1] *= rh;
			return *this;
		}
		inline Vec2& operator /= (const Vec2& rh)
		{
			_e[0] /= rh.X();
			_e[1] /= rh.Y();
			return *this;
		}
		inline Vec2& operator /= (const double& rh)
		{
			_e[0] /= rh;
			_e[1] /= rh;
			return *this;
		}

		inline bool operator == (const Vec2& other) const
		{
			return _e[0] == other.X() && _e[1] == other.Y();
		}

		inline bool operator != (const Vec2& other) const
		{
			return _e[0] != other.X() && _e[1] != other.Y();
		}

		//Vector math functions
		inline double DotProduct(const Vec2& b) const
		{
			return ((_e[0] * b.X()) + (_e[1] * b.Y()));
		}

		inline double Length() const
		{
			return sqrt(_e[0] * _e[0] + _e[1] * _e[1]);
		}

		inline double SqrLength() const
		{
			return _e[0] * _e[0] + _e[1] * _e[1];
		}

		inline Vec2 UnitVector() const
		{
			return *this / this->Length();
		}

		inline void MakeUnitVector()
		{
			double k = 1.0 / this->Length();
			_e[0] *= k;
			_e[1] *= k;
		}

	private:
		double _e[2];
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
			_inverseDir[0] = 1 / dir.X();
			_inverseDir[1] = 1 / dir.Y();
			_inverseDir[2] = 1 / dir.Z();

			_signs[0] = (_inverseDir.X() < 0);
			_signs[1] = (_inverseDir.Y() < 0);
			_signs[2] = (_inverseDir.Z() < 0);
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

	class ColourArray
	{
	public:
		ColourArray() = delete;
		ColourArray(int columns, int rows) : _rows(rows), _columns(columns), _colours(std::vector<sf::Color>(rows * columns, sf::Color(0,0,0,255))) { }

		sf::Color& GetColourAtPosition(int& x, int& y)
		{
			return _colours.at(y * _columns + x);
		}

		void ColourPixelAtPosition(int& x, int& y, sf::Color col)
		{
			_colours.at(y * _columns + x) = col;
		}

		void ColourPixelAtIndex(int ind, sf::Color col)
		{
			_colours.at(ind) = col;
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
		AA::Vec3 lh(a.X() * (1 - t), a.Y() * (1 - t), a.Z() * (1 - t));
		AA::Vec3 rh(b.X() * t, b.Y() * t, b.Z() * t);
		return AA::Vec3(lh.X() + rh.X(), lh.Y() + rh.Y(), lh.Z() + rh.Z());
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

	static sf::Color NormalToColour(AA::Vec3 norm)
	{
		norm += 1;
		norm *= 0.5;
		return norm.Vec3ToCol();
	}


	//Quicker fMin due to not needing to check for NaNs and other exceptions
	static double dMin(double a, double b)
	{
		return a < b ? a : b;
	}

	//Quicker fMax due to not needing to check for NaNs and other exceptions
	static double dMax(double a, double b)
	{
		return a > b ? a : b;
	}

	static double InverseLerp(double a, double b, double v)
	{
		return (v - a) / (b - a);
	}

	static double Lerp(double a, double b, double t)
	{
		return (1.0f - t) * a + b * t;
	}

	static sf::Color LinearTonemap(Vec3 hdr)
	{
		hdr[0] = hdr[0] > 1.0 ? 1.0 : hdr[0];
		hdr[1] = hdr[1] > 1.0 ? 1.0 : hdr[1];
		hdr[2] = hdr[2] > 1.0 ? 1.0 : hdr[2];

		hdr[0] = hdr[0] < 0.0 ? 0.0 : hdr[0];
		hdr[1] = hdr[1] < 0.0 ? 0.0 : hdr[1];
		hdr[2] = hdr[2] < 0.0 ? 0.0 : hdr[2];

		return hdr.Vec3ToCol();
	}

	static sf::Color GammaTonemap(Vec3 hdr)
	{
		double power = 1 / 2.2;

		hdr[0] = std::min(std::pow(hdr[0], power), 1.0);
		hdr[1] = std::min(std::pow(hdr[1], power), 1.0);
		hdr[2] = std::min(std::pow(hdr[2], power), 1.0);

		return hdr.Vec3ToCol();
	}

	static AA::Vec3 LinearTonemapHDR(Vec3 hdr)
	{
		hdr[0] = hdr[0] > 1.0 ? 1.0 : hdr[0];
		hdr[1] = hdr[1] > 1.0 ? 1.0 : hdr[1];
		hdr[2] = hdr[2] > 1.0 ? 1.0 : hdr[2];

		hdr[0] = hdr[0] < 0.0 ? 0.0 : hdr[0];
		hdr[1] = hdr[1] < 0.0 ? 0.0 : hdr[1];
		hdr[2] = hdr[2] < 0.0 ? 0.0 : hdr[2];

		return hdr;
	}

	static AA::Vec3 GammaTonemapHDR(Vec3 hdr)
	{
		double power = 1 / 2.2;

		hdr[0] = std::min(std::pow(hdr[0], power), 1.0);
		hdr[1] = std::min(std::pow(hdr[1], power), 1.0);
		hdr[2] = std::min(std::pow(hdr[2], power), 1.0);

		return hdr;
	}
}

//Hash for Vec2
template<> struct std::hash<AA::Vec2>
{
	size_t operator()(AA::Vec2 const& vec) const
	{
		return ( (std::hash<double>()(vec.X()) ^ (std::hash<double>()(vec.Y()) << 1)) );
	}
};

//Hash for Vec3
template<> struct std::hash<AA::Vec3>
{
	size_t operator()(AA::Vec3 const& vec) const
	{
		return ((std::hash<double>()(vec.X()) ^ (std::hash<double>()(vec.Y()) << 1)) >> 1) ^ (std::hash<double>()(vec.Z()) << 1);
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
