#include "PathFinder.h"


PathFinder::PathFinder(World* ownerWorld) : OwnerWorld(ownerWorld),
											DestinationPoint(ZeroVector)
{
}


PathFinder::~PathFinder(void)
{
}

CalculationPoint::CalculationPoint(PathPoint* point, float g, float h, CalculationPoint* cameFrom)
{
	F = g + h;
	G = g;
	H = h;
	Point = point;
	CameFrom = cameFrom;
}

void PathFinder::ReconstructPath(CalculationPoint* start, CalculationPoint* end)
{
	Path.clear();
	CalculationPoint *thisPoint = end;
	while (thisPoint != NULL)
	{
		Path.insert(Path.begin(), thisPoint->Point); // back order
		thisPoint = thisPoint->CameFrom;
	}

	// free closed set
	while (ClosedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *ClosedSet.begin();
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		ClosedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
	}
}

bool PathFinder::CreateNewPath(Vector2D startPoint, Vector2D endPoint)
{
	DestinationPoint = endPoint;

	// find first and last path points
	float minDistStart = 1000000.f;
	float minDistEnd = minDistStart;
	PathPoint *firstPoint = NULL, *lastPoint = NULL;

	for (std::set<PathPoint*>::iterator it = OwnerWorld->NavigationMap.begin(), end = OwnerWorld->NavigationMap.end(); it != end; it++)
	{
		if (((*it)->Location - startPoint).Size() < minDistStart)
		{
			firstPoint = (*it);
			minDistStart = (firstPoint->Location - startPoint).Size();
		}

		if (((*it)->Location - endPoint).Size() < minDistEnd)
		{
			lastPoint = (*it);
			minDistEnd = (lastPoint->Location - endPoint).Size();
		}
	}

	WARN_IF(!firstPoint, "PATHFINDER: Nearest PathPoint not found");
	WARN_IF(!lastPoint, "PATHFINDER: Target PathPoint not found");

	std::set<CalculationPoint*> OpenSet;

	// insert first point
	CalculationPoint *cFirstPoint = new CalculationPoint(firstPoint, 0.f, (firstPoint->Location - endPoint).Size(), NULL);
	OpenSet.insert(cFirstPoint);

	// while open set is not empty
	while (OpenSet.size() > 0)
	{
		float minF = 1000000.0;
		CalculationPoint *currentPoint;
		// find point with minimal heuristic
		for (std::set<CalculationPoint*>::iterator it = OpenSet.begin(), end = OpenSet.end(); it != end; it++)
		{
			WARN_IF(*it == NULL, "PATHFINDER: NULL pointer in OpenSet");
			if ((*it)->F < minF)
			{
				minF = (*it)->F;
				currentPoint = (*it);
			}
		}

		// move point to closed set from open set
		ClosedSet.insert(currentPoint);
		OpenSet.erase(currentPoint);

		// if we got it
		if (currentPoint->Point == lastPoint)
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
			ReconstructPath(cFirstPoint, currentPoint);

			return true;
		}

		for (std::set<PathPoint*>::iterator it = currentPoint->Point->LegalPoints.begin(), end = currentPoint->Point->LegalPoints.end(); it != end; it++)
		{
			bool bPointInClosedSet = false;
			PathPoint* neighbor = (*it);
			WARN_IF(!neighbor, "PATHFINDER: NULL pointer 'neightbor'");
			for (std::set<CalculationPoint*>::iterator it2 = ClosedSet.begin(), end = ClosedSet.end(); it2 != end; it2++)
			{
				if ((*it2)->Point == neighbor)
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
			
			float g = currentPoint->G + (currentPoint->Point->Location - neighbor->Location).Size();
			float h = (lastPoint->Location - neighbor->Location).Size();
			float f = g + h;
			bool bPointInOpenSet = false;
			CalculationPoint* openPoint;
			for (std::set<CalculationPoint*>::iterator it2 = OpenSet.begin(), end = OpenSet.end(); it2 != end; it2++)
			{
				WARN_IF(*it2 == NULL, "PATHFINDER: NULL pointer in OpenSet");
				if ((*it2)->Point == neighbor)
				{
					bPointInOpenSet = true;
					openPoint = (*it2);
					break;
				}
			}
			
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
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL PathPoint in OpenSet");
		OpenSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
	}

	// free closed set
	while (ClosedSet.size() > 0)
	{
		CalculationPoint* pointToDelete = *ClosedSet.begin();
		WARN_IF(pointToDelete == NULL, "PATHFINDER: Error in pathfinding! NULL or duplicated PathPoint");
		ClosedSet.erase(pointToDelete);
		delete pointToDelete;
		pointToDelete = NULL;
	}

	return false;
}

Vector2D PathFinder::GetNextPoint()
{
	if (Path.size() != 0)
	{
		PathPoint * next = *(Path.begin());
		Path.remove(next);
		return next->Location;
	}
	else
	{
		return DestinationPoint;
	}
}