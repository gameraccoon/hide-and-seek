#include "Modules/Collide.h"

#include <stdlib.h>
#include <cmath>

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>

namespace Collide
{
	static float SignedArea(const Vector2D &a, const Vector2D &b, const Vector2D &c)
	{
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}

	enum class ResistDir
	{
		Normal,
		PointA,
		PointB
	};

	bool IsCollideGeometry(const Hull& hull1, const Hull& hull2, const Vector2D& center1, const Vector2D& center2, Vector2D& outResist)
	{
		// circle vs circle
		if (hull1.type == Hull::Type::Circular && hull2.type == Hull::Type::Circular)
		{
			float dist = (center1 - center2).qSize() - (hull1.getQRadius() + hull2.getQRadius());
			if (dist <= 0.0f)
			{
				outResist = (center1 - center2) - (center1 - center2).ort() * (hull1.getRadius() + hull2.getRadius());
				return true;
			}
			return false;
		}
		// circle vs rect
		else if (hull1.type == Hull::Type::Circular || hull2.type == Hull::Type::Circular)
		{
			const Hull *cHull, *rHull;
			const Vector2D *cCenter, *rCenter;
			if (hull1.type == Hull::Type::Circular)
			{
				cHull = &hull1;
				cCenter = &center1;
				rHull = &hull2;
				rCenter = &center2;
			}
			else
			{
				cHull = &hull2;
				cCenter = &center2;
				rHull = &hull1;
				rCenter = &center1;
			}

			const Border *nearestBorder = nullptr;
			float nearestBorderQDistance = MAXFLOAT;
			ResistDir nearestBorderResistDir = ResistDir::Normal;
			for (auto &border : rHull->borders)
			{
				Vector2D borderA = *rCenter + border.getA();

				float qDistance = DotProduct(border.getNormal(), borderA - *cCenter);
				qDistance*=qDistance;

				if (qDistance < nearestBorderQDistance)
				{
					ResistDir resistDir = ResistDir::Normal;

					Vector2D borderB = *rCenter + border.getB();
					float distA = (*cCenter - borderA).qSize();
					float distB = (*cCenter - borderB).qSize();

					// find nearest point
					if (distA < distB)
					{
						// check if we outside the section
						if (DotProduct(*cCenter - borderA, border.getB() - border.getA()) < 0)
						{
							qDistance = distA;
							resistDir = ResistDir::PointA;
						}
					}
					else
					{
						if (DotProduct(*cCenter - borderB, border.getA() - border.getB()) < 0)
						{
							qDistance = distB;
							resistDir = ResistDir::PointB;
						}
					}

					if (qDistance < nearestBorderQDistance)
					{
						nearestBorder = &border;
						nearestBorderQDistance = qDistance;
						nearestBorderResistDir = resistDir;
					}
				}
			}

			// if not intersecting the border
			if (nearestBorderQDistance >= cHull->getQRadius())
			{
				// if we outside the figure
				if (nearestBorder == nullptr
					|| nearestBorderResistDir != ResistDir::Normal
					|| SignedArea(*cCenter,
						*rCenter + nearestBorder->getA(),
						*rCenter + nearestBorder->getB()) <= 0.0f)
				{
					return false;
				}
			}

			// we're touching the border or fully inside the figure
			Vector2D resist = ZERO_VECTOR;
			switch (nearestBorderResistDir)
			{
			case ResistDir::Normal:
				resist = nearestBorder->getNormal() * (sqrt(nearestBorderQDistance) - cHull->getRadius());
				break;
			case ResistDir::PointA:
			{
				Vector2D diffA = (*cCenter - *rCenter - nearestBorder->getA());
				resist = diffA - diffA.ort() * cHull->getRadius();
			}
				break;
			case ResistDir::PointB:
			{
				Vector2D diffB = (*cCenter - *rCenter - nearestBorder->getB());
				resist = diffB - diffB.ort() * cHull->getRadius();
				break;
			}
			default:
				break;
			}

			if (cHull == &hull1)
			{
				outResist=-resist;
			}
			else
			{
				outResist=resist;
			}
			return true;
		}
		else
		{
			// ToDo :some checks here
		}
		return false;
	}

	bool doCollide(const CollisionComponent* collisionA, const Vector2D& locationA,
				   const CollisionComponent* collisionB, const Vector2D& locationB, Vector2D& outResist)
	{
		// get AABB of the actors
		BoundingBox box = collisionA->getBoundingBox();
		BoundingBox ourBox = collisionB->getBoundingBox();
		// if the actor's AABB intersects with the Man's AABB (in new Man location)
		if ((box.minX < ourBox.maxX
			&& ourBox.minX < box.maxX)
			&& (box.minY < ourBox.maxY
			&& ourBox.minY < box.maxY))
		{
			return IsCollideGeometry(collisionA->getGeometry(), collisionB->getGeometry(),
				locationA, locationB, outResist);
		}
		return false;
	}

	void updateOriginalBoundingBox(CollisionComponent* collision)
	{
		const Hull& geometry = collision->getGeometry();

		if (geometry.type == Hull::Type::Circular)
		{
			float radius = geometry.getRadius();
			collision->setOriginalBoundingBox(BoundingBox(Vector2D(-radius, -radius), Vector2D(radius, radius)));
		}
		else
		{
			float minX = 1000000;
			float maxX = -1000000;
			float minY = 1000000;
			float maxY = -1000000;

			for (auto point : geometry.points)
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

			collision->setOriginalBoundingBox(BoundingBox(Vector2D(minX, minY), Vector2D(maxX, maxY)));
		}
	}

}
