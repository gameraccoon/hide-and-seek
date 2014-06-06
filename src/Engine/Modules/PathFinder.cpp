#include "PathFinder.h"

#include <Debug/DebugMethods.h>

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

PathFinder::PathFinder(World* world) : destinationPoint(ZERO_VECTOR),
    ownerWorld(world)
{
}


PathFinder::~PathFinder(void)
{
}

CalculationPoint::CalculationPoint(PathPoint* point, float g, float h, CalculationPoint* cameFrom)
{
	this->f = g + h;
	this->g = g;
	this->h = h;
	this->point = point;
	this->cameFrom = cameFrom;
}

void PathFinder::reconstructPath(CalculationPoint* start, CalculationPoint* end)
{
	this->path.clear();
	CalculationPoint *currentPoint = end;
	while (currentPoint != nullptr)
	{
		this->path.insert(this->path.begin(), currentPoint->point); // back order
		currentPoint = currentPoint->cameFrom;
	}

	// free closed set
	while (this->closedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *this->closedSet.begin();
		WARN_IF(pointToDelete == nullptr, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		this->closedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}
}

bool PathFinder::createNewPath(Vector2D startPoint, Vector2D endPoint)
{
	this->destinationPoint = endPoint;

	// find first and last path points
	float minDistStart = 1000000.f;
	float minDistEnd = minDistStart;
	PathPoint *firstPoint = nullptr, *lastPoint = nullptr;

	for (auto const pathPoint : this->ownerWorld->navigationMap)
	{
		if ((pathPoint->location - startPoint).size() < minDistStart)
		{
			firstPoint = pathPoint;
			minDistStart = (firstPoint->location - startPoint).size();
		}

		if ((pathPoint->location - endPoint).size() < minDistEnd)
		{
			lastPoint = pathPoint;
			minDistEnd = (lastPoint->location - endPoint).size();
		}
	}

	WARN_IF(!firstPoint, "PATHFINDER: Nearest PathPoint not found");
	WARN_IF(!lastPoint, "PATHFINDER: Target PathPoint not found");

	std::set<CalculationPoint*> OpenSet;

	// insert first point
	CalculationPoint *cFirstPoint = new CalculationPoint(firstPoint, 0.f, (firstPoint->location - endPoint).size(), nullptr);
	OpenSet.insert(cFirstPoint);

	// while open set is not empty
	while (OpenSet.size() > 0)
	{
		float minF = 1000000.0;
		CalculationPoint *currentPoint;
		// find point with minimal heuristic
		for (auto const &openSetPoint : OpenSet)
		{
			if (openSetPoint->f < minF)
			{
				minF = openSetPoint->f;
				currentPoint = openSetPoint;
			}
		}

		// move point to closed set from open set
		this->closedSet.insert(currentPoint);
		OpenSet.erase(currentPoint);

		// if we got it
		if (currentPoint->point == lastPoint)
		{
			// free open set
			while (OpenSet.size() > 0)
			{
				CalculationPoint* pointToDelete = *OpenSet.begin();
				WARN_IF(pointToDelete == nullptr, "Error in pathfinding! NULL PathPoint in OpenSet");
				OpenSet.erase(pointToDelete);
				delete pointToDelete;
				pointToDelete = nullptr;
			}

			WARN_IF(!cFirstPoint, "PATHFINDER: NULL pointer 'cFirstPoint'");
			WARN_IF(!currentPoint, "PATHFINDER: NULL pointer 'currentPoint'");
			// create final path
			this->reconstructPath(cFirstPoint, currentPoint);

			return true;
		}

		for (auto const &neighbor : currentPoint->point->legalPoints)
		{
			bool bPointInClosedSet = false;
			for (auto const &alreadyFound : this->closedSet)
			{
				if (neighbor == alreadyFound->point)
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
			
			float g = currentPoint->g + (currentPoint->point->location - neighbor->location).size();
			float h = (lastPoint->location - neighbor->location).size();
			float f = g + h;
			bool bPointInOpenSet = false;
			CalculationPoint* openPoint = nullptr;
			for (auto const &openSetPoint : OpenSet)
			{
				if (openSetPoint->point == neighbor)
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
				if (f < openPoint->f)
				{
					openPoint->f = f;
					openPoint->g = g;
					openPoint->h = h;
					openPoint->cameFrom = currentPoint;
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
		WARN_IF(pointToDelete == nullptr, "PATHFINDER: Error in pathfinding! NULL PathPoint in OpenSet");
		OpenSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}

	// free closed set
	while (this->closedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *this->closedSet.begin();
		WARN_IF(pointToDelete == nullptr, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		this->closedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = nullptr;
	}

	return false;
}

Vector2D PathFinder::getNextPoint()
{
	if (this->path.size() != 0)
	{
		PathPoint * next = *(this->path.begin());
		this->path.remove(next);
		return next->location;
	}
	else
	{
		return this->destinationPoint;
	}
}
