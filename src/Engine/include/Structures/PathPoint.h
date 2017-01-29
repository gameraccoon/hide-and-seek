#pragma once

#include <vector>
#include <memory>
#include "Core/Vector2D.h"

class PathPoint
{
public:
	using Ptr = std::unique_ptr<PathPoint>;

public:
	PathPoint(const Vector2D& location);
	/** Add point to the legal points list */
	void connect(PathPoint* destPoint);

public:
	/** Location of this point in a world (points don't know to which world they belong to) */
	Vector2D location;
	/** Points at which we can go from this point */
	std::vector<PathPoint*> legalPoints;
};

/** Two sided connect */
inline void ConnectTwoPoints(PathPoint* a, PathPoint* b)
{
	a->connect(b);
	b->connect(a);
}
