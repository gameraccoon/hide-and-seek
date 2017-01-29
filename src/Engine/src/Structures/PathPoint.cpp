#include "Structures/PathPoint.h"


PathPoint::PathPoint(const Vector2D& location)
	: location(location)
{
}

void PathPoint::connect(PathPoint* destPoint)
{
	legalPoints.push_back(destPoint);
}
