#include "BoundingBox.h"


BoundingBox::BoundingBox(float minX, float minY, float maxX, float maxY)
{
	this->minX = minX;
	this->minY = minY;
	this->maxX = maxX;
	this->maxY = maxY;
}

BoundingBox::BoundingBox(Vector2D minPoint, Vector2D maxPoint)
{
	this->minX = minPoint.x;
	this->minY = minPoint.y;
	this->maxX = maxPoint.x;
	this->maxY = maxPoint.y;
}

BoundingBox::~BoundingBox(void)
{
}

const Vector2D BoundingBox::getFirst() const
{
	return Vector2D(this->minX, this->minY);
}

const Vector2D BoundingBox::getThird() const
{
	return Vector2D(this->maxX, this->maxY);
}

const Vector2D BoundingBox::getSecond() const
{
	return Vector2D(this->maxX, this->minY);
}

const Vector2D BoundingBox::getFourth() const
{
	return Vector2D(this->minX, this->maxY);
}
