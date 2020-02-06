#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

Mesh::Mesh(const char* path)
{
	LoadModel(path);
}

bool Mesh::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	return false;
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
