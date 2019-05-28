#include "Utils/PathFinder.h"


PathFinder::PathFinder(World* world)
	: mDestinationPoint(ZERO_VECTOR)
	, mOwnerWorld(world)
{
}


PathFinder::~PathFinder()
{
}

CalculationPoint::CalculationPoint(PathPoint* point, float g, float h, CalculationPoint* cameFrom)
	: Point(point)
	, F(g + h)
	, G(g)
	, H(h)
	, CameFrom(cameFrom)
{
}

void PathFinder::reconstructPath(CalculationPoint* end)
{
	mPath.clear();
	CalculationPoint *currentPoint = end;
	while (currentPoint != nullptr)
	{
		mPath.insert(mPath.begin(), currentPoint->Point); // back order
		currentPoint = currentPoint->CameFrom;
	}

	// free closed set
	while (mClosedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *mClosedSet.begin();
		mClosedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}
}

bool PathFinder::createNewPath(Vector2D /*startPoint*/, Vector2D endPoint)
{
	mDestinationPoint = endPoint;

	// find first and last path points
//	float minDistStart = 1000000.f;
//	float minDistEnd = minDistStart;
	PathPoint *firstPoint = nullptr, *lastPoint = nullptr;

//	for (auto const& pathPoint : mOwnerWorld->getNavigationMap())
//	{
//		if ((pathPoint->location - startPoint).size() < minDistStart)
//		{
//			firstPoint = pathPoint.get();
//			minDistStart = (firstPoint->location - startPoint).size();
//		}

//		if ((pathPoint->location - endPoint).size() < minDistEnd)
//		{
//			lastPoint = pathPoint.get();
//			minDistEnd = (lastPoint->location - endPoint).size();
//		}
//	}

	std::set<CalculationPoint*> OpenSet;

	// insert first point
	CalculationPoint *cFirstPoint = new CalculationPoint(firstPoint, 0.f, (firstPoint->location - endPoint).size(), nullptr);
	OpenSet.insert(cFirstPoint);

	// while open set is not empty
	while (OpenSet.size() > 0)
	{
		float minF = 1000000.0;
		CalculationPoint *currentPoint = nullptr;
		// find point with minimal heuristic
		for (const auto& openSetPoint : OpenSet)
		{
			if (openSetPoint->F < minF)
			{
				minF = openSetPoint->F;
				currentPoint = openSetPoint;
			}
		}

		if (currentPoint == nullptr)
		{
			return false;
		}

		// move point to closed set from open set
		mClosedSet.insert(currentPoint);
		OpenSet.erase(currentPoint);

		// if we got it
		if (currentPoint->Point == lastPoint)
		{
			// free open set
			while (OpenSet.size() > 0)
			{
				CalculationPoint* pointToDelete = *OpenSet.begin();
				OpenSet.erase(pointToDelete);
				delete pointToDelete;
				pointToDelete = nullptr;
			}

			// create final path
			reconstructPath(currentPoint);

			return true;
		}

		for (auto const &neighbor : currentPoint->Point->legalPoints)
		{
			bool bPointInClosedSet = false;
			for (auto const &alreadyFound : mClosedSet)
			{
				if (neighbor == alreadyFound->Point)
				{
					bPointInClosedSet = true;
					break;
				}
			}

			// skip if point already in closed set
			if (bPointInClosedSet)
			{
				continue;
			}
			
			float g = currentPoint->G + (currentPoint->Point->location - neighbor->location).size();
			float h = (lastPoint->location - neighbor->location).size();
			float f = g + h;
			bool bPointInOpenSet = false;
			CalculationPoint* openPoint = nullptr;
			for (auto const &openSetPoint : OpenSet)
			{
				if (openSetPoint->Point == neighbor)
				{
					bPointInOpenSet = true;
					openPoint = openSetPoint;
					break;
				}
			}

			if (openPoint == nullptr)
				break;
			
			// skip if we have better path for this point
			if (bPointInOpenSet)
			{
				if (f < openPoint->F)
				{
					openPoint->F = f;
					openPoint->G = g;
					openPoint->H = h;
					openPoint->CameFrom = currentPoint;
				}
			}
			else
			{
				OpenSet.insert(new CalculationPoint(neighbor, g, h, currentPoint));
			}
		}
	}

	// free open set
	while (OpenSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *OpenSet.begin();
		OpenSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}

	// free closed set
	while (mClosedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *mClosedSet.begin();
		mClosedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}

	return false;
}

Vector2D PathFinder::getNextPoint()
{
	if (mPath.size() != 0)
	{
		PathPoint * next = *(mPath.begin());
		mPath.remove(next);
		return next->location;
	}
	else
	{
		return mDestinationPoint;
	}
}
