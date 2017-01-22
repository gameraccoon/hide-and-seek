#include "Structures/Hull.h"


Hull::Hull()
{
}

Hull::~Hull()
{
}

void Hull::generateBorders()
{
	borders.clear();
	int count = (int)points.size();
	for (int i = 0; i < count; i++)
	{
		borders.insert(borders.end(), Border(points[i], points[(i + 1) % count]));
	}
}

float Hull::getQRadius() const
{
	return qRadius;
}

float Hull::getRadius() const
{
	return radius;
}

void Hull::setRadius(float newRadius)
{
	radius = newRadius;
	qRadius = newRadius * newRadius;
}
