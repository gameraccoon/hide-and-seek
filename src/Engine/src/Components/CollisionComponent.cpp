#include "Components/CollisionComponent.h"

/*
void Actor::updateCollision()
{
	if (getGeometry()->type == Hull::Type::Circular)
	{
		float radius = getGeometry()->getRadius();
		setColideBox(BoundingBox(getLocation() - Vector2D(radius, radius), getLocation() + Vector2D(radius, radius)));
	}
	else
	{
		float minX = 1000;
		float maxX = -1000;
		float minY = 1000;
		float maxY = -1000;

		for (auto point : mGeometry.points)
		{
			if (point.x < minX)
			{
				minX = point.x;
			}

			if (point.x > maxX)
			{
				maxX = point.x;
			}

			if (point.y < minY)
			{
				minY = point.y;
			}

			if (point.y > maxY)
			{
				maxY = point.y;
			}
		}

		setColideBox(BoundingBox(getLocation() + Vector2D(minX, minY), getLocation() + Vector2D(maxX, maxY)));
	}
}

void Actor::setGeometry(Hull newGeometry)
{
	mInitialGeometry = newGeometry;
}

void Actor::updateGeometry()
{
	Hull localGeometry(mInitialGeometry);
	localGeometry.points.clear();
	for (auto point : mInitialGeometry.points)
	{
		Vector2D scaled(point);
		scaled.x = point.x * mScale.x;
		scaled.y = point.y * mScale.y;

		Vector2D rotated = Vector2D(scaled.rotation() + mDirection).ort() * scaled.size();

		localGeometry.points.insert(localGeometry.points.end(), rotated);
	}
	localGeometry.generateBorders();
	mGeometry = localGeometry;

	updateCollision();
}*/

CollisionComponent::~CollisionComponent()
{

}
