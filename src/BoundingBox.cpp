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

Vector2D BoundingBox::GetFirst()
{
	return Vector2D(MinX, MinY);
}

Vector2D BoundingBox::GetThird()
{
	return Vector2D(MaxX, MaxY);
}

Vector2D BoundingBox::GetSecond()
{
	return Vector2D(MaxX, MinY);
}

Vector2D BoundingBox::GetFourth()
{
	return Vector2D(MinX, MaxY);
}