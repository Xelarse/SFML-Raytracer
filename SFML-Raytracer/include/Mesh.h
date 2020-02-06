#pragma once
#include "Hittable.h"
#include "Utilities.h"

class Mesh : public Hittable
{
public:
	Mesh() = delete;
	Mesh(const char* path);
	~Mesh() = default;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
private:

	bool LoadModel(const char* path);
	
	std::vector<AA::Vertex> _verts;
	std::vector<uint32_t> _inds;

	sf::Texture _texture;
};

