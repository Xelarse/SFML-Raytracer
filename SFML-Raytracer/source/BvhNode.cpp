#include "..\include\BvhNode.h"
#include <iostream>

BvhNode::BvhNode(Hittable** dataPtr, int dataSize, double t0, double t1)
{
	ConstructBVH(dataPtr, dataSize, t0, t1);
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

void BvhNode::ConstructBVH(Hittable** dataPtr, int dataSize, double t0, double t1)
{
	//Done every update to corrispond with objects moving

	//Randomly choose an axis to divide on
	int axis = static_cast<int>(3 * AA::RanDouble());

	//Sort on the random axis
	if (axis == 0)
	{
		qsort(dataPtr, dataSize, sizeof(Hittable*), BvhNode::CompareXBox);
	}
	else if (axis == 1)
	{
		qsort(dataPtr, dataSize, sizeof(Hittable*), BvhNode::CompareYBox);
	}
	else
	{
		qsort(dataPtr, dataSize, sizeof(Hittable*), BvhNode::CompareZBox);
	}

	//If theres only one or two hittables deal with the exceptions, otherwise recursively make another set of bvh's
	if (dataSize == 1)
	{
		_left = _right = dataPtr[0];
	}
	else if (dataSize == 2)
	{
		_left = dataPtr[0];
		_right = dataPtr[1];
	}
	else
	{
		_left = new BvhNode(dataPtr, dataSize / 2, t0, t1);
		_right = new BvhNode(dataPtr + dataSize / 2, dataSize - (dataSize / 2), t0, t1);
	}


	//Set this bvh box to encompas the two lower nodes
	AABB leftBox, rightBox;

	if (!_left->BoundingBox(t0, t1, leftBox) || !_right->BoundingBox(t0, t1, rightBox))
	{
		std::cout << "No bounding box in BvhNode Constructor!" << std::endl;
	}

	_box = AABB::SurroundingBox(leftBox, rightBox);
}

int BvhNode::CompareXBox(const void* a, const void* b)
{
	AABB leftBox, rightBox;

	//Need to do this ptr malarkey to get around the const
	Hittable* lhs = *(Hittable**)a;
	Hittable* rhs = *(Hittable**)b;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().X() - rightBox.Min().X() < 0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int BvhNode::CompareYBox(const void* a, const void* b)
{
	AABB leftBox, rightBox;

	//Need to do this ptr malarkey to get around the const
	Hittable* lhs = *(Hittable**)a;
	Hittable* rhs = *(Hittable**)b;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().Y() - rightBox.Min().Y() < 0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int BvhNode::CompareZBox(const void* a, const void* b)
{
	AABB leftBox, rightBox;

	//Need to do this ptr malarkey to get around the const
	Hittable* lhs = *(Hittable**)a;
	Hittable* rhs = *(Hittable**)b;

	if (!lhs->BoundingBox(0, 0, leftBox) || !rhs->BoundingBox(0, 0, rightBox))
	{
		std::cout << "No AABB in the BvhNode Constructor" << std::endl;
	}

	if (leftBox.Min().Z() - rightBox.Min().Z() < 0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
