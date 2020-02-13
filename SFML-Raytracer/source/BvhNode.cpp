#include "..\include\BvhNode.h"
#include <iostream>

BvhNode::BvhNode(std::vector<Hittable*> hittables, double t0, double t1, bool useSmart)
{
	ConstructBVH(hittables, t0, t1, useSmart);
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

void BvhNode::ConstructBVH(std::vector<Hittable*> hittables, double t0, double t1, bool useSmart)
{
	//TODO Fix the smart construction or just leave it 
	if (useSmart)
	{
		SmartConstruction(hittables, t0, t1);
	}
	else
	{
		DumbConstruction(hittables, t0, t1);
	}
}

void BvhNode::DumbConstruction(std::vector<Hittable*> hittables, double t0, double t1)
{
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

		std::vector<Hittable*> right;

		for (int i = 0; i < hittables.size() / 2; ++i)
		{
			right.push_back(hittables.back());
			hittables.pop_back();
		}

		_left = new BvhNode(hittables, t0, t1, false);
		_right = new BvhNode(right, t0, t1, false);
	}


	//Set this bvh box to encompas the two lower nodes
	AABB leftBox, rightBox;

	if (!_left->BoundingBox(t0, t1, leftBox) || !_right->BoundingBox(t0, t1, rightBox))
	{
		std::cout << "No bounding box in BvhNode Constructor!" << std::endl;
	}

	_box = AABB::SurroundingBox(leftBox, rightBox);
}

void BvhNode::SmartConstruction(std::vector<Hittable*> hittables, double t0, double t1)
{
	//If theres only one or two hittables deal with the exceptions, otherwise recursively make another set of bvh's
	//TODO Add cost for making a leaf cause currently its not better
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
		std::array<int, 3> inds {-1, -1, -1};
		std::array<double, 3> costs {-1, -1, -1};
		std::vector<Hittable*> rhs;

		//Go through each axis find the best place on that axis to split the index array and cache the cost
		CalculateAxisCost(hittables, AxisSort::X_AXIS, costs[0], inds[0]);
		CalculateAxisCost(hittables, AxisSort::Y_AXIS, costs[1], inds[1]);
		CalculateAxisCost(hittables, AxisSort::Z_AXIS, costs[2], inds[2]);

		//Compare the three axis costs and choose the cheapest cost
		int lowestInd = 0;
		for (int i = 1; i < costs.size(); ++i)
		{
			if (costs[i] > 0 && costs[i] < costs[lowestInd])
			{
				lowestInd = i;
			}
		}

		//Sort the array again on that axis and split it at the index cached
		switch (static_cast<AxisSort>(lowestInd))
		{
			case AxisSort::X_AXIS:
			{
				std::sort(hittables.begin(), hittables.end(), BvhNode::CompareXBox);
				break;
			}
			case AxisSort::Y_AXIS:
			{
				std::sort(hittables.begin(), hittables.end(), BvhNode::CompareYBox);
				break;
			}
			default:
			{
				std::sort(hittables.begin(), hittables.end(), BvhNode::CompareZBox);
				break;
			}
		}

		//Safety check to set a default split of center if something messed up from earlier
		inds[lowestInd] = inds[lowestInd] == -1 ? hittables.size() / 2 : inds[lowestInd];
		int popOffs = hittables.size() - (inds[lowestInd] + 1);

		for (int i = 0; i < popOffs; ++i)
		{
			rhs.push_back(hittables.back());
			hittables.pop_back();
		}

		_left = new BvhNode(hittables, t0, t1, true);
		_right = new BvhNode(rhs, t0, t1, true);
	}


	//Set this bvh box to encompas the two lower nodes
	AABB leftBox, rightBox;

	if (!_left->BoundingBox(t0, t1, leftBox) || !_right->BoundingBox(t0, t1, rightBox))
	{
		std::cout << "No bounding box in BvhNode Constructor!" << std::endl;
	}

	_box = AABB::SurroundingBox(leftBox, rightBox);
}

void BvhNode::CalculateAxisCost(std::vector<Hittable*> hittables, AxisSort axis, double& outCost, int& outIndex)
{
	AABB minBox, maxBox, tempBox;
	double min, max;
	int axisInd;
	outCost = 20000.0;

	axisInd = static_cast<int>(axis);

	switch (axis)
	{
		case AxisSort::X_AXIS:
			std::sort(hittables.begin(), hittables.end(), BvhNode::CompareXBox);
			break;
		case AxisSort::Y_AXIS:
			std::sort(hittables.begin(), hittables.end(), BvhNode::CompareYBox);
			break;
		case AxisSort::Z_AXIS:
			std::sort(hittables.begin(), hittables.end(), BvhNode::CompareZBox);
			break;
		default:
			std::cout << "Invalid axis inputted for sorting BVH !" << std::endl;
			break;
	}

	hittables.front()->BoundingBox(0, 0, maxBox);
	hittables.back()->BoundingBox(0, 0, minBox);
	min = minBox.Min()[axisInd];
	max = maxBox.Max()[axisInd];

	for (int i = 0; i < hittables.size(); ++i)
	{
		//Get the current percentage along at this index
		hittables[i]->BoundingBox(0, 0, tempBox);
		double percent = 1 - AA::InverseLerp(min, max, tempBox.Min()[axisInd]);

		//Math
		// Cost of a split = (Probab of hitting box A  * Tris in box A) + (Probab of hitting box B * tris in box B)
		//use the math function to determine the weighting of the percentage and the current iteration vs the remaining percentage and the remaining iterations
		double newCost = (percent * (i + 1.0)) + ((1.0 - percent) * (hittables.size() - (i + 1.0)));

		//if the cost is lower than the current one cache the cost and save the index
		if (newCost < outCost)
		{
			outCost = newCost;
			outIndex = i;
		}
	}
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
