#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>

Mesh::Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale, bool isStatic, bool useBvh, bool useSmart) 
	: Hittable(isStatic, sf::Color(255,255,255,255), false),  _position(position), _scale(scale), _useBvh(useBvh), _useSah(useSmart)
{
	LoadModel(path);
	UpdateTrisPosition();
	UpdateTrisScale();

	if (_useBvh)
	{
		_meshBvh = std::make_unique<BvhNode>(_tris, 0.0, 0.0, _useSah);
	}
}

Mesh::~Mesh()
{
	for (auto& hit : _tris)
	{
		delete hit;
	}
}

bool Mesh::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	if (_tris.size() == 0)
	{
		return false;
	}

	Hittable::HitResult tempRes;
	bool didHit = false;
	double closestHit = t_max;

	////With BVH
	//Create a bvh of the hittables, then do the iterations of the ray against the bvh intersect ray
	if (_useBvh)
	{
		//If the objects can move then we need to remake the bvh, TODO Set a dirty flag later so this isnt done everyyyyy update
		if (!_isStatic || !_meshBvh->IsConstructed())
		{
			_meshBvh->ConstructBVH(_tris, 0.0, 0.0, _useSah);
		}
		didHit = _meshBvh->IntersectedRay(ray, t_min, t_max, tempRes);

		if (didHit)
		{
			res = tempRes;
		}
	}

	//// Without BVH
	else
	{
		for (auto& hitt : _tris)
		{
			if (hitt->IntersectedRay(ray, t_min, closestHit, tempRes))
			{
				didHit = true;
				closestHit = tempRes.t;
				res = tempRes;
			}
		}
	}

	return didHit;
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
	std::vector<AA::Vertex> verts;
	std::vector<uint32_t> inds;

	for (const auto& shape : shapes)
	{
		if (shape.mesh.indices.size() % 3 != 0)
		{
			std::cout << "Error Loading Obj!" << std::endl << "Model at path: '" << path << "' doesn't have inds divisible by 3 to make correct TRIs" << std::endl;
			return false;
		}
		for (int i = 0; i < shape.mesh.indices.size(); i+=3)
		{
			std::array<AA::Vertex, 3> verts;
			auto index = shape.mesh.indices;

			////Retrieve the vertex information from the loaded attributes

			////Vertex 1 --------------------------------------------------------
			//Position
			verts[0]._position[0] = attributes.vertices[3 * index[i].vertex_index + 0];
			verts[0]._position[1] = attributes.vertices[3 * index[i].vertex_index + 1];
			verts[0]._position[2] = attributes.vertices[3 * index[i].vertex_index + 2];

			//Normal
			verts[0]._normal[0] = attributes.normals[3 * index[i].normal_index + 0];
			verts[0]._normal[1] = attributes.normals[3 * index[i].normal_index + 1];
			verts[0]._normal[2] = attributes.normals[3 * index[i].normal_index + 2];

			//Tex cords
			verts[0]._texCord[0] = attributes.texcoords[2 * index[i].texcoord_index + 0];
			verts[0]._texCord[1] = attributes.texcoords[2 * index[i].texcoord_index + 1];

			////Vertex 2 --------------------------------------------------------
			//Position
			verts[1]._position[0] = attributes.vertices[3 * index[i + 1].vertex_index + 0];
			verts[1]._position[1] = attributes.vertices[3 * index[i + 1].vertex_index + 1];
			verts[1]._position[2] = attributes.vertices[3 * index[i + 1].vertex_index + 2];

			//Normal
			verts[1]._normal[0] = attributes.normals[3 * index[i + 1].normal_index + 0];
			verts[1]._normal[1] = attributes.normals[3 * index[i + 1].normal_index + 1];
			verts[1]._normal[2] = attributes.normals[3 * index[i + 1].normal_index + 2];

			//Tex cords
			verts[1]._texCord[0] = attributes.texcoords[2 * index[i + 1].texcoord_index + 0];
			verts[1]._texCord[1] = attributes.texcoords[2 * index[i + 1].texcoord_index + 1];

			////Vertex 3 --------------------------------------------------------
			//Position
			verts[2]._position[0] = attributes.vertices[3 * index[i + 2].vertex_index + 0];
			verts[2]._position[1] = attributes.vertices[3 * index[i + 2].vertex_index + 1];
			verts[2]._position[2] = attributes.vertices[3 * index[i + 2].vertex_index + 2];

			//Normal
			verts[2]._normal[0] = attributes.normals[3 * index[i + 2].normal_index + 0];
			verts[2]._normal[1] = attributes.normals[3 * index[i + 2].normal_index + 1];
			verts[2]._normal[2] = attributes.normals[3 * index[i + 2].normal_index + 2];

			//Tex cords
			verts[2]._texCord[0] = attributes.texcoords[2 * index[i + 2].texcoord_index + 0];
			verts[2]._texCord[1] = attributes.texcoords[2 * index[i + 2].texcoord_index + 1];

			////Create the Tri and push it back onto vector
			_tris.push_back(new Triangle(verts, _position, _scale, _isStatic));
		}
	}

	return true;
}

void Mesh::UpdateTrisPosition()
{
	for (auto& tri : _tris)
	{
		tri->Move(_position);
	}
}

void Mesh::UpdateTrisScale()
{
	for (auto& tri : _tris)
	{
		tri->Scale(_scale);
	}
}

bool Mesh::BoundingBox(double t0, double t1, AABB& outBox) const
{
	if (_tris.size() < 1)
	{
		return false;
	}

	AABB tempBox;
	bool didExpand = false;

	if (!_tris.front()->BoundingBox(t0, t1, tempBox))
	{
		return false;
	}
	else
	{
		//Loop over all objects in the scene and expand the surrounding bounding box
		outBox = tempBox;
		for (int i = 1; i < _tris.size(); i++)
		{
			if (_tris[i]->BoundingBox(t0, t1, tempBox))
			{
				didExpand = true;
				outBox = AABB::SurroundingBox(outBox, tempBox);
			}
		}
	}

	//If the box never expanded just the box for the first object will do for later calcs, a full scene wide one isnt needed
	return didExpand;
}

void Mesh::Move(AA::Vec3 newPos)
{
	if(_isStatic) { return; }
	_position = newPos;
	UpdateTrisPosition();
}

void Mesh::Scale(AA::Vec3 newScale)
{
	if(_isStatic) { return; }
	_scale = newScale;
	UpdateTrisScale();
}
