#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>
#include <set>
#include "../src/Globals.h"
#include "../src/World.h"
#include "../src/Vector2D.h"
#include "../src/PathPoint.h"

class CalculationPoint
{
public:
	CalculationPoint(PathPoint* point, float g, float h, CalculationPoint* cameFrom);
	PathPoint* point;
	float f, g, h;
	CalculationPoint* cameFrom;
};

/**
 * Class allows find path for some man.
 */
class PathFinder
{
public:
	PathFinder(World* ownerWorld);
	~PathFinder(void);
	
	/** Find and save path from startPoint to endPoint in owner world */
	bool createNewPath(Vector2D startPoint, Vector2D endPoint);
	/**
	 * Get location of a next point of this path.
	 * @return next point or DestinationPoint if no more points of this path.
	 */
	Vector2D getNextPoint();
private:
	/** Current path */
	std::list<PathPoint*> path;
	Vector2D destinationPoint;
	World* ownerWorld;
	std::set<CalculationPoint*> closedSet;
	void reconstructPath(CalculationPoint* start, CalculationPoint* end);
};

#endif
