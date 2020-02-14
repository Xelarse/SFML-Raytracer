#include "..\include\Triangle.h"

Triangle::Triangle(std::array<AA::Vertex, 3> verts, AA::Vec3 position, AA::Vec3 scale, bool isStatic, sf::Color col, bool useColour) : Hittable(isStatic, col, useColour), _verts(verts), _pos(position), _scale(scale)
{
	//Set up the bounds for the Tri
	_bounds[0] = _verts[0]._position;
	_bounds[1] = _verts[0]._position;

	for (int i = 1; i < _verts.size(); ++i)
	{
		//Min
		_bounds[0][0] = _verts[i]._position.X() < _bounds[0][0] ? _verts[i]._position.X() : _bounds[0][0];
		_bounds[0][1] = _verts[i]._position.Y() < _bounds[0][1] ? _verts[i]._position.Y() : _bounds[0][1];
		_bounds[0][2] = _verts[i]._position.Y() < _bounds[0][2] ? _verts[i]._position.Z() : _bounds[0][2];

		//Max
		_bounds[1][0] = _verts[i]._position.X() > _bounds[1][0] ? _verts[i]._position.X() : _bounds[1][0];
		_bounds[1][1] = _verts[i]._position.Y() > _bounds[1][1] ? _verts[i]._position.Y() : _bounds[1][1];
		_bounds[1][2] = _verts[i]._position.Z() > _bounds[1][2] ? _verts[i]._position.Z() : _bounds[1][2];
	}
}

Triangle::~Triangle()
{
}

bool Triangle::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle

	//Check against each tri using Muller Trumbore?
	// RESEARCH IT FOR THE REPORT HERE https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	//Get the verts of the triangle
	AA::Vertex v0 = _verts[0];
	AA::Vertex v1 = _verts[1];
	AA::Vertex v2 = _verts[2];

	//Barycentric co ords
	double u, v;

	////Apply position and scale
	v0._position *= _scale;
	v1._position *= _scale;
	v2._position *= _scale;

	v0._position += _pos;
	v1._position += _pos;
	v2._position += _pos;

	//Calc planes normal
	AA::Vec3 v0v1 = v1._position - v0._position;
	AA::Vec3 v0v2 = v2._position - v0._position;
	AA::Vec3 pvec = ray._dir.CrossProduct(v0v2);
	float det = v0v1.DotProduct(pvec);

	//Check if the ray misses or if it hits a backfacing tri by checking if determinant is close or below zero
	if (det < AA::kEpsilon)
	{
		return false;
	}

	float invDet = 1 / det;

	AA::Vec3 tvec = ray._startPos - v0._position;
	u = tvec.DotProduct(pvec) * invDet;
	if (u < 0 || u > 1)
	{
		return false;
	}

	AA::Vec3 qvec = tvec.CrossProduct(v0v1);
	v = ray._dir.DotProduct(qvec) * invDet;
	if (v < 0 || u + v > 1)
	{
		return false;
	}

	//At this point its passed all tests and hit the TRI
	res.t = v0v2.DotProduct(qvec) * invDet;
	res.p = ray.GetPointAlongRay(res.t);
	res.normal = v0._normal;
	res.col = AA::NormalToColour(res.normal);
	return true;
}

bool Triangle::BoundingBox(double t0, double t1, AABB& outBox) const
{
	outBox = AABB(
		(_bounds[0] * _scale) + _pos,
		(_bounds[1] * _scale) + _pos
	);

	return true;
}

void Triangle::Move(AA::Vec3 newPos)
{
    if(_isStatic) { return; }
    _pos = newPos;
}

void Triangle::Scale(AA::Vec3 newScale)
{
    if(_isStatic) { return; }
    _scale = newScale;
}
