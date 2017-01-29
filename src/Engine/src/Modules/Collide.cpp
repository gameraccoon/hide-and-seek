#include "Modules/Collide.h"

namespace Collide
{
	inline float SignedArea(const Vector2D &a, const Vector2D &b, const Vector2D &c)
	{
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}

	bool IsCollideGeometry(const Hull* hull1, const Hull* hull2, const Vector2D& center1, const Vector2D& center2)
	{
		if (hull1->type == Hull::Type::Circular && hull2->type == Hull::Type::Circular)
		{
			return abs(hull1->getRadius() + hull2->getRadius()) - (center1 - center2).size() > 0.0f;
		}
		else if (hull1->type == Hull::Type::Circular || hull2->type == Hull::Type::Circular)
		{
			const Hull *cHull, *aHull;
			const Vector2D *cCenter, *aCenter;
			if (hull1->type == Hull::Type::Circular)
			{
				cHull = hull1;
				cCenter = &center1;
				aHull = hull2;
				aCenter = &center2;
			}
			else
			{
				cHull = hull2;
				cCenter = &center2;
				aHull = hull1;
				aCenter = &center1;
			}

			const Border *nearestBorder = nullptr;
			float nearestBorderDistance = 0.0f;
			for (auto &border : aHull->borders)
			{
				float distance = (*aCenter + (border.getA() + border.getB())/2.0f - *cCenter).size();
				if (nearestBorder == nullptr || distance < nearestBorderDistance)
				{
					nearestBorder = &border;
					nearestBorderDistance = distance;
				}
			}

			float dist1 = (*aCenter + nearestBorder->getA() - *cCenter).size();
			float dist2 = (*aCenter + nearestBorder->getB() - *cCenter).size();

			if (dist1 < cHull->getRadius() || dist2 < cHull->getRadius())
			{
				return true;
			}
		
			for (auto &border : aHull->borders)
			{
				if (SignedArea(*cCenter - (border.getNormal() * cHull->getRadius()),
					*aCenter + border.getA(),
					*aCenter + border.getB()) < 0.0f)
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			// some checking
		}
		return false;
	}

	bool isWillCollide(const IActor* actor1, const World* world, Vector2D step)
	{
		// for each actors in the world
		for (const auto& actor : world->getAllActors())
		{
			// if the actor is not this man
			if (actor.get() != actor1
				&& (actor->getType() != ActorType::Light
				&& actor->getType() != ActorType::Special
				&& actor->getType() != ActorType::Bullet))
			{
				// get an actor's AABB (axis-aligned bounding box)
				BoundingBox box = actor->getBoundingBox();
				BoundingBox ourBox = actor1->getBoundingBox();
				// if the actor's AABB intersects with the Man's AABB (in new Man location)
				if ((box.minX < ourBox.maxX + step.x
						&& ourBox.minX + step.x < box.maxX)
					&& (box.minY < ourBox.maxY + step.y
						&& ourBox.minY + step.y  < box.maxY))
				{
					// actor's path is not free
					return IsCollideGeometry(actor1->getGeometry(), actor->getGeometry(),
						actor1->getLocation() + step, actor->getLocation());
				}
			}
		}

		return false;
	}
}
