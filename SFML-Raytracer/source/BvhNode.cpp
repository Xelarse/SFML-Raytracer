#include "..\include\BvhNode.h"
#include <iostream>

BvhNode::BvhNode(std::vector<Hittable*> hittables, double t0, double t1)
{
	ConstructBVH(hittables, t0, t1);
}

bool BvhNode::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
	if (_box.IntersectedRay(ray, t_min, t_max))
	{
		HitResult leftRes, rightRes;
		bool hitLeft = _left->IntersectedRay(ray, t_min, t_max, leftRes);
		bool hitRight = _right->IntersectedRay(ray, t_min, t_max, rightRes);

		if (hitLeft && hitRight)
		{
			if (leftRes.t < rightRes.t)
			{
				res = leftRes;
			}
			else
			{
				res = rightRes;
			}
			return true;
		}
		else if (hitLeft)
		{
			res = leftRes;
			return true;
		}
		else if (hitRight)
		{
			res = rightRes;
			return true;
		}

		return false;
	}

	return false;
}

bool BvhNode::BoundingBox(double t0, double t1, AABB& outBox) const
{
	outBox = _box;
	return true;
}

void BvhNode::ConstructBVH(std::vector<Hittable*> hittables, double t0, double t1)
{
	//Randomly choose an axis to divide on
	int axis = static_cast<int>(3 * AA::RanDouble());

	//Sort on the random axis
	if (axis == 0)
	{
		std::sort(hittables.begin(), hittables.end(), BvhNode::CompareXBox);
	}
	else if (axis == 1)
	{
		std::sort(hittables.begin(), hittables.end(), BvhNode::CompareYBox);
	}
	else
	{
		std::sort(hittables.begin(), hittables.end(), BvhNode::CompareZBox);
	}

	//If theres only one or two hittables deal with the exceptions, otherwise recursively make another set of bvh's
	if (hittables.size() == 1)
	{
		_left = _right = hittables[0];
	}
	else if (hittables.size() == 2)
	{
		_left = hittables.front();
		_right = hittables.back();
	}
	else
	{
		std::vector<Hittable*> right;

		for (int i = 0; i < hittables.size() / 2; ++i)
		{
			right.push_back(hittables.back());
			hittables.pop_back();
		}

		_left = new BvhNode(hittables, t0, t1);
		_right = new BvhNode(right, t0, t1);
	}


	//Set this bvh box to encompas the two lower nodes
	AABB leftBox, rightBox;

	if (!_left->BoundingBox(t0, t1, leftBox) || !_right->BoundingBox(t0, t1, rightBox))
	{
		std::cout << "No bounding box in BvhNode Constructor!" << std::endl;
	}

	_box = AABB::SurroundingBox(leftBox, rightBox);
}

int BvhNode::CompareXBox(Hittable* lhs, Hittable* rhs)
{
	AABB leftBox, rightBox;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().X() - rightBox.Min().X() < 0.0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int BvhNode::CompareYBox(Hittable* lhs, Hittable* rhs)
{
	AABB leftBox, rightBox;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().Y() - rightBox.Min().Y() < 0.0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int BvhNode::CompareZBox(Hittable* lhs, Hittable* rhs)
{
	AABB leftBox, rightBox;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().Z() - rightBox.Min().Z() < 0.0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
