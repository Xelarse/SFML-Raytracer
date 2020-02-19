#pragma once
#include "Hittable.h"
#include "Triangle.h"
#include "Utilities.h"
#include "BvhNode.h"

class Mesh : public Hittable
{
public:
	enum class ModelParams
	{
		FLIP_X,
		FLIP_Y,
		FLIP_Z,
		DEFAULT
	};


	Mesh() = delete;
	Mesh(const char* modelPath, const char* texturePath, AA::Vec3 position, AA::Vec3 scale, bool isStatic, bool useBvh = false, bool useSmart = false, ModelParams param = ModelParams::DEFAULT, Light* sceneLight = nullptr);
	~Mesh() override;

	bool IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res) override;
	bool BoundingBox(double t0, double t1, AABB& outBox) const override;

	void Move(AA::Vec3 newPos) override;
	void Scale(AA::Vec3 newScale) override;

private:
	bool LoadModel(const char* path, ModelParams param);
	bool LoadTexture(const char* path);
	void UpdateTrisPosition();
	void UpdateTrisScale();
	
	AA::Vec3 _position = AA::Vec3();
	AA::Vec3 _scale = AA::Vec3();
	
	std::vector<Hittable*> _tris;

	std::unique_ptr<sf::Image> _meshTexture;

	std::unique_ptr<BvhNode> _meshBvh;
	const bool _useBvh;
	const bool _useSah;
};

