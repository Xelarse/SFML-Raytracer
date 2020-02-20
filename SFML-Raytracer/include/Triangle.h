#pragma once
#include "Hittable.h"

class Triangle : public Hittable
{
public:
	Triangle() = delete;
	Triangle(std::array<AA::Vertex, 3> verts, AA::Vec3 position, AA::Vec3 scale, sf::Image* texPtr, bool isStatic, Material mat, Light* sceneLight = nullptr);
	~Triangle() override;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

private:

	sf::Color GetPixelColour(double u, double v);

	const std::array<AA::Vertex, 3> _verts;
	AA::Vec3 _pos;
	AA::Vec3 _scale;
	sf::Image* _texturePtr;
	std::array<AA::Vec3, 2> _bounds;
};

