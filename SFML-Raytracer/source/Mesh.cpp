#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

Mesh::Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale) : _position(position), _scale(scale)
{
	LoadModel(path);
}

Mesh::Mesh(AA::Vec3 position, AA::Vec3 scale) : _position(position), _scale(scale)
{
	//Load in a single tri
	AA::Vertex v0 = AA::Vertex(AA::Vec3(0, 0, 0), AA::Vec3(0, 0, -1), AA::Vec2(0, 0));
	AA::Vertex v1 = AA::Vertex(AA::Vec3(0, 1, 0), AA::Vec3(0, 0, -1), AA::Vec2(0, 0));
	AA::Vertex v2 = AA::Vertex(AA::Vec3(0, 0, -1), AA::Vec3(0, 0, -1), AA::Vec2(0, 0));

	_verts.push_back(v0);
	_verts.push_back(v1);
	_verts.push_back(v2);

	_inds.push_back(0);
	_inds.push_back(1);
	_inds.push_back(2);
}

bool Mesh::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle

	//Double check we actually have a full set if TRIs in the inds
	if (_inds.size() % 3 != 0)
	{
		return false;
	}

	//Start the hit off really high so it will only be improved or ignored
	//Ensure that the res for the p is a zero vector so that we can do a check later to see if its been changed
	double closestHit = t_max;

	//Check against each tri using Muller Trumbore?
	// RESEARCH IT FOR THE REPORT HERE https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	for (size_t i = 0; i < _inds.size(); i+=3)
	{
		//Get the verts of the triangle
		AA::Vertex v0 = _verts[_inds[i]];
		AA::Vertex v1 = _verts[_inds[i + 1]];
		AA::Vertex v2 = _verts[_inds[i + 2]];

		//Barycentric co ords
		double u, v;

		////Apply position and scale
		v0._position *= _scale;
		v1._position *= _scale;
		v2._position *= _scale;

		v0._position += _position;
		v1._position += _position;
		v2._position += _position;

		//Calc planes normal
		AA::Vec3 v0v1 = v1._position - v0._position;
		AA::Vec3 v0v2 = v2._position - v0._position;
		AA::Vec3 pvec = ray._dir.CrossProduct(v0v2);
		float det = v0v1.DotProduct(pvec);

		//Check if the ray misses or if it hits a backfacing tri by checking if determinant is close or below zero
		if (det < AA::kEpsilon) { continue; }

		float invDet = 1 / det;

		AA::Vec3 tvec = ray._startPos - v0._position;
		u = tvec.DotProduct(pvec) * invDet;
		if (u < 0 || u > 1) { continue; };

		AA::Vec3 qvec = tvec.CrossProduct(v0v1);
		v = ray._dir.DotProduct(qvec) * invDet;
		if (v < 0 || u + v > 1) { continue; }

		double t = v0v2.DotProduct(qvec) * invDet;

		//If it passes all of these conditions then one final check to make sure its the nearest tri and write it out
		if (t < closestHit)
		{
			res.t = closestHit = t;
			res.p = ray.GetPointAlongRay(res.t);
			res.normal = v0._normal;
		}
	}

	//Check if res is not set to default and if they aint return true
	return closestHit < t_max;
}

bool Mesh::LoadModel(const char* path)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, path))
	{
		std::cout << "Error loading Obj! Info below:" << std::endl << "Warning: " << warn << std::endl << "Error: " << err << std::endl;
		return false;
	}

	std::unordered_map<AA::Vertex, uint32_t> uniqueVerts = {};

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			AA::Vertex vert = AA::Vertex();

			////Retrieve the vertex information from the loaded attributes

			//Position
			vert._position._x = attributes.vertices[3 * index.vertex_index + 0];
			vert._position._y = attributes.vertices[3 * index.vertex_index + 1];
			vert._position._z = attributes.vertices[3 * index.vertex_index + 2];

			//Normal
			vert._normal._x = attributes.normals[3 * index.normal_index + 0];
			vert._normal._y = attributes.normals[3 * index.normal_index + 1];
			vert._normal._z = attributes.normals[3 * index.normal_index + 2];

			//Tex cords
			vert._texCord._x = attributes.texcoords[2 * index.texcoord_index + 0];
			vert._texCord._y = attributes.texcoords[2 * index.texcoord_index + 1];


			//// We only wanna store unique verts so check if this vert already exists, if it do just add the index in the indicies, if not push it
			if (uniqueVerts.count(vert) == 0)
			{
				uniqueVerts[vert] = static_cast<uint32_t>(_verts.size());
				_verts.push_back(vert);
			}

			//Push the ind to match the vert
			_inds.push_back(uniqueVerts[vert]);
		}
	}

	return true;
}
