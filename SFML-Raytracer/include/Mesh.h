#pragma once
#include "Hittable.h"
#include "Triangle.h"
#include "Utilities.h"
#include "BvhNode.h"

class Mesh : public Hittable
{
public:
	Mesh() = delete;
	Mesh(const char* path, AA::Vec3 position, AA::Vec3 scale, bool isStatic);
	~Mesh() override = default;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

private:
	bool LoadModel(const char* path);
	
	AA::Vec3 _position = AA::Vec3();
	AA::Vec3 _scale = AA::Vec3();
	
	std::vector<Triangle> _tris;
	std::array<AA::Vec3, 2> _bounds;

	sf::Texture _texture;

	std::unique_ptr<BvhNode> _meshBvh;
};

