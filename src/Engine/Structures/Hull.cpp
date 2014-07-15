#include "Hull.h"


Hull::Hull(void)
{
}

Hull::~Hull(void)
{
}

void Hull::generateBorders()
{
	this->borders.clear();
	int count = this->points.size();
	for (int i = 0; i < count; i++)
	{
		this->borders.insert(this->borders.end(), Border(this->points[i], this->points[(i + 1) % count]));
	}
}

float Hull::getQRadius() const
{
	return this->qRadius;
}

float Hull::getRadius() const
{
	return this->radius;
}

void Hull::setRadius(float newRadius)
{
	this->radius = newRadius;
	this->qRadius = newRadius * newRadius;
}
