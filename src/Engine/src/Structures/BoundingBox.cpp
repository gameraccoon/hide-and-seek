#include "Structures/BoundingBox.h"


BoundingBox::BoundingBox(float minX, float minY, float maxX, float maxY)
{
	minX = minX;
	minY = minY;
	maxX = maxX;
	maxY = maxY;
}

BoundingBox::BoundingBox(Vector2D minPoint, Vector2D maxPoint)
{
	minX = minPoint.x;
	minY = minPoint.y;
	maxX = maxPoint.x;
	maxY = maxPoint.y;
}

BoundingBox::~BoundingBox()
{
}

Vector2D BoundingBox::getFirst()
{
	return Vector2D(minX, minY);
}

Vector2D BoundingBox::getThird()
{
	return Vector2D(maxX, maxY);
}

Vector2D BoundingBox::getSecond()
{
	return Vector2D(maxX, minY);
}

Vector2D BoundingBox::getFourth()
{
	return Vector2D(minX, maxY);
}
