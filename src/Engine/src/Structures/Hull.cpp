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
    for (size_t i = 0, count = points.size(); i < count; i++)
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
