#include "PathFinder.h"

#include "../Configs/DebugMethods.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

PathFinder::PathFinder(World* ownerWorld) : ownerWorld(ownerWorld),
	destinationPoint(ZERO_VECTOR)
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
	while (currentPoint != NULL)
	{
		this->path.insert(this->path.begin(), currentPoint->point); // back order
		currentPoint = currentPoint->cameFrom;
	}

	// free closed set
	while (this->closedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *this->closedSet.begin();
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		this->closedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
	}
}

bool PathFinder::createNewPath(Vector2D startPoint, Vector2D endPoint)
{
	this->destinationPoint = endPoint;

	// find first and last path points
	float minDistStart = 1000000.f;
	float minDistEnd = minDistStart;
	PathPoint *firstPoint = NULL, *lastPoint = NULL;

	for (std::set<PathPoint*>::iterator i = this->ownerWorld->navigationMap.begin(), iEnd = ownerWorld->navigationMap.end(); i != iEnd; i++)
	{
		if (((*i)->location - startPoint).size() < minDistStart)
		{
			firstPoint = (*i);
			minDistStart = (firstPoint->location - startPoint).size();
		}

		if (((*i)->location - endPoint).size() < minDistEnd)
		{
			lastPoint = (*i);
			minDistEnd = (lastPoint->location - endPoint).size();
		}
	}

	WARN_IF(!firstPoint, "PATHFINDER: Nearest PathPoint not found");
	WARN_IF(!lastPoint, "PATHFINDER: Target PathPoint not found");

	std::set<CalculationPoint*> OpenSet;

	// insert first point
	CalculationPoint *cFirstPoint = new CalculationPoint(firstPoint, 0.f, (firstPoint->location - endPoint).size(), NULL);
	OpenSet.insert(cFirstPoint);

	// while open set is not empty
	while (OpenSet.size() > 0)
	{
		float minF = 1000000.0;
		CalculationPoint *currentPoint;
		// find point with minimal heuristic
		for (std::set<CalculationPoint*>::iterator i = OpenSet.begin(), iEnd = OpenSet.end(); i != iEnd; i++)
		{
			WARN_IF(*i == NULL, "PATHFINDER: NULL pointer in OpenSet");
			if ((*i)->f < minF)
			{
				minF = (*i)->f;
				currentPoint = (*i);
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
				WARN_IF(pointToDelete == NULL, "Error in pathfinding! NULL PathPoint in OpenSet");
				OpenSet.erase(pointToDelete);
				delete pointToDelete;
				pointToDelete = NULL;
			}

			WARN_IF(!cFirstPoint, "PATHFINDER: NULL pointer 'cFirstPoint'");
			WARN_IF(!currentPoint, "PATHFINDER: NULL pointer 'currentPoint'");
			// create final path
			this->reconstructPath(cFirstPoint, currentPoint);

			return true;
		}

		for (std::set<PathPoint*>::iterator i = currentPoint->point->legalPoints.begin(),
			iEnd = currentPoint->point->legalPoints.end(); i != iEnd; i++)
		{
			bool bPointInClosedSet = false;
			PathPoint* neighbor = (*i);
			WARN_IF(!neighbor, "PATHFINDER: NULL pointer 'neightbor'");
			for (std::set<CalculationPoint*>::iterator j = this->closedSet.begin(), jEnd = this->closedSet.end(); j != jEnd; j++)
			{
				if ((*j)->point == neighbor)
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
			CalculationPoint* openPoint;
			for (std::set<CalculationPoint*>::iterator j = OpenSet.begin(), jEnd = OpenSet.end(); j != jEnd; j++)
			{
				WARN_IF(*j == NULL, "PATHFINDER: NULL pointer in OpenSet");
				if ((*j)->point == neighbor)
				{
					bPointInOpenSet = true;
					openPoint = (*j);
					break;
				}
			}
			
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
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL PathPoint in OpenSet");
		OpenSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
	}

	// free closed set
	while (this->closedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *this->closedSet.begin();
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		this->closedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
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