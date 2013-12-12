#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>
#include "../src/World.h"
#include "../src/Vector2D.h"
#include "../src/PathPoint.h"

/**
 * Class allows find path for some man.
 */
class PathFinder
{
public:
	PathFinder(World ownerWorld);
	~PathFinder(void);
	
	/** Find and save path from startPoint to endPoint in owner world */
	bool CreateNewPath(Vector2D startPoint, Vector2D endPoint);
	/**
	 * Get location of a next point of this path.
	 * @return next point or ZeroVector if no more points of this path.
	 */
	Vector2D GetNextPoint();
private:
	/** Current path */
	std::list<PathPoint> Path;
};

#endif