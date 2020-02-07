#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

Mesh::Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale) : _position(position), _scale(scale)
{
	LoadModel(path);
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

	//Check against each tri
	for (size_t i = 0; i < _inds.size(); i+=3)
	{
		//Get the verts of the triangle
		AA::Vertex v0 = _verts[_inds[i]];
		AA::Vertex v1 = _verts[_inds[i + 1]];
		AA::Vertex v2 = _verts[_inds[i + 2]];

		//Apply position and scale
		v0._position += _position;
		v1._position += _position;
		v2._position += _position;

		v0._position *= _scale;
		v1._position *= _scale;
		v2._position *= _scale;

		//Calc planes normal
		AA::Vec3 v0v1 = v1._position - v0._position;
		AA::Vec3 v0v2 = v2._position - v0._position;
		AA::Vec3 pNorm = v0v1.CrossProduct(v0v2);
		double area = pNorm.Length();

		//Check if the ray is parallel to the tri first
		double pNormalDotRayDir = pNorm.DotProduct(ray._dir);
		if (pNormalDotRayDir >= 0 && pNormalDotRayDir <= 0.01) { continue; }

		//Next compute t and do a check to make sure the tri isnt behind the ray origin
		double d = pNorm.DotProduct(v0._position);
		double t = (pNorm.DotProduct(ray._startPos) + d);
		if (t < t_min || t > t_max) { continue; }


		//Calc P using previous formulas now we got t
		AA::Vec3 p = ray._startPos + (ray._dir * t);

		//Check if the position is within all 3 edges of the tri
		AA::Vec3 C;

		//Edge 1
		AA::Vec3 edge1 = v1._position - v0._position;
		AA::Vec3 vp1 = p - v0._position;
		C = edge1.CrossProduct(vp1);
		if (pNorm.DotProduct(C) < 0) { continue; }


		//Edge 2
		AA::Vec3 edge2 = v2._position - v1._position;
		AA::Vec3 vp2 = p - v1._position;
		C = edge2.CrossProduct(vp2);
		if (pNorm.DotProduct(C) < 0) { continue; }

		//Edge 3
		AA::Vec3 edge3 = v0._position - v2._position;
		AA::Vec3 vp3 = p - v2._position;
		C = edge3.CrossProduct(vp3);
		if (pNorm.DotProduct(C) < 0) { continue; }


		//If it passes all of these conditions then one final check to make sure its the nearest tri and write it out
		if (t < closestHit)
		{
			res.t = closestHit = t;
			res.p = p;
			res.normal = pNorm;
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
