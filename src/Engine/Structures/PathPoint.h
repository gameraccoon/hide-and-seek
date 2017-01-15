#pragma once

#include <set>
#include "../Core/Vector2D.h"

class PathPoint
{
public:
	PathPoint(Vector2D location);
	virtual ~PathPoint();
	/** Add point to the legal points list */
	void connect(PathPoint* destPoint);
	/** Location of this point in a world (points don't know to which world they belongs) */
	Vector2D location;
	/** Points at which we can go from this point */
	std::set<PathPoint*> legalPoints;
};

/** Two sided connect */
inline void ConnectTwoPoints(PathPoint* a, PathPoint* b)
{
	a->connect(b);
	b->connect(a);
}
