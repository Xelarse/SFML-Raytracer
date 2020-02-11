#pragma once
#include "Hittable.h"
#include "Utilities.h"

class Mesh : public Hittable
{
public:
	Mesh() = delete;
	Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale, bool isStatic);
	Mesh(AA::Vec3 position, AA::Vec3 scale);
	~Mesh() override = default;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

private:
	bool LoadModel(const char* path);
	
	AA::Vec3 _position = AA::Vec3();
	AA::Vec3 _scale = AA::Vec3();
	
	std::vector<AA::Vertex> _verts;
	std::vector<uint32_t> _inds;

	sf::Texture _texture;
};

