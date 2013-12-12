#include "PathPoint.h"


PathPoint::PathPoint(Vector2D location) : Location(location)
{
}


PathPoint::~PathPoint(void)
{
}


void PathPoint::Connect(PathPoint* destPoint)
{
	LegalPoints.insert(destPoint);
}