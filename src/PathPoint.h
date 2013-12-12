#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <list>
#include "../src/Vector2D.h"

class PathPoint
{
public:
	PathPoint(Vector2D location);
	virtual ~PathPoint(void);
	/** Location of this point in a world (points don't know to which world they belongs) */
	Vector2D Location;
	/** Points at which we can go from this point */
	std::list<PathPoint*> LegalPoints;
};

#endif