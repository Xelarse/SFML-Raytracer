#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

Mesh::Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale, bool isStatic) : Hittable(isStatic, sf::Color(255,255,255,255), false),  _position(position), _scale(scale)
{
	LoadModel(path);
}

bool Mesh::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	//TODO use the BVH built around the model for intersections, Potentialy extend BVH to include an offset so all nodes are applied with that offset
}

bool Mesh::LoadModel(const char* path)
{
	//TODO make sure you cache the lowest and highest values for bounds of the whole model

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
			vert._position[0] = attributes.vertices[3 * index.vertex_index + 0];
			vert._position[1] = attributes.vertices[3 * index.vertex_index + 1];
			vert._position[2] = attributes.vertices[3 * index.vertex_index + 2];

			//Normal
			vert._normal[0] = attributes.normals[3 * index.normal_index + 0];
			vert._normal[1] = attributes.normals[3 * index.normal_index + 1];
			vert._normal[2] = attributes.normals[3 * index.normal_index + 2];

			//Tex cords
			vert._texCord[0] = attributes.texcoords[2 * index.texcoord_index + 0];
			vert._texCord[1] = attributes.texcoords[2 * index.texcoord_index + 1];


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

bool Mesh::BoundingBox(double t0, double t1, AABB& outBox) const
{
	//TODO add function to calc this later but for now meshes are gonna ignore the bounding box whilst i work out a way to process tris with it
	return false;
}

void Mesh::Move(AA::Vec3 newPos)
{
	if(_isStatic) { return; }
	_position = newPos;
}

void Mesh::Scale(AA::Vec3 newScale)
{
	if(_isStatic) { return; }
	_scale = newScale;
}
