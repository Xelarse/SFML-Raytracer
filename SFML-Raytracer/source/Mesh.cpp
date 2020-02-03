#include "..\include\Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(const char* path)
{
}

bool Mesh::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	return false;
}

bool Mesh::LoadModel(const char* path)
{
	//TODO set up with https://vulkan-tutorial.com/Loading_models
	//Once this is done do Tri intersect based of this mesh
	//Then BVH boi
	return false;
}
