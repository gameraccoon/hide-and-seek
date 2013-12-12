#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <set>
#include "../src/Vector2D.h"

class PathPoint
{
public:
	PathPoint(Vector2D location);
	virtual ~PathPoint(void);
	/** Add point to the legal points list */
	void Connect(PathPoint* destPoint);
	/** Location of this point in a world (points don't know to which world they belongs) */
	Vector2D Location;
	/** Points at which we can go from this point */
	std::set<PathPoint*> LegalPoints;
};

#endif