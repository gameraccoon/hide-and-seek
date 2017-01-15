#include "PathPoint.h"


PathPoint::PathPoint(Vector2D location) : location(location)
{
}


PathPoint::~PathPoint()
{
}


void PathPoint::connect(PathPoint* destPoint)
{
	legalPoints.insert(destPoint);
}