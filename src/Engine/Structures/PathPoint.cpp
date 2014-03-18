#include "PathPoint.h"


PathPoint::PathPoint(Vector2D location) : location(location)
{
}


PathPoint::~PathPoint(void)
{
}


void PathPoint::connect(PathPoint* destPoint)
{
	this->legalPoints.insert(destPoint);
}