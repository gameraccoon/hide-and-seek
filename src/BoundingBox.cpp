#include "BoundingBox.h"


BoundingBox::BoundingBox(float minX, float minY, float maxX, float maxY)
{
	MinX = minX;
	MinY = minY;
	MaxX = maxX;
	MaxY = maxY;
}

BoundingBox::BoundingBox(Vector2D minPoint, Vector2D maxPoint)
{
	MinX = minPoint.X;
	MinY = minPoint.Y;
	MaxX = maxPoint.X;
	MaxY = maxPoint.Y;
}

BoundingBox::~BoundingBox(void)
{
}

Vector2D BoundingBox::GetMin()
{
	return Vector2D(MinX, MinY);
}

Vector2D BoundingBox::GetMax()
{
	return Vector2D(MaxX, MaxY);
}