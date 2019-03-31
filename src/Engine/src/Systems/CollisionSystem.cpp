#include "Systems/CollisionSystem.h"

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <Modules/Collide.h>

namespace {
	static void UpdateOriginalBoundingBox(const CollisionComponent::Ptr& collision)
	{
		const Hull& geometry = collision->getGeometry();

		if (geometry.type == Hull::Type::Circular)
		{
			float radius = geometry.getRadius();
			collision->setOriginalBoundingBox(BoundingBox(Vector2D(radius, radius), Vector2D(radius, radius)));
		}
		else
		{
			float minX = 1000;
			float maxX = -1000;
			float minY = 1000;
			float maxY = -1000;

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

void CollisionSystem::update(World* world, float /*dt*/)
{
	auto components = world->getComponents<CollisionComponent, TransformComponent>();

	for (auto component : components)
	{
		auto& collisionComponent = std::get<0>(component);
		auto& transformComponent = std::get<1>(component);

		if (collisionComponent->isBoundingBoxDirty())
		{
			UpdateOriginalBoundingBox(collisionComponent);
		}

		collisionComponent->setBoundingBox(collisionComponent->getOriginalBoundingBox() + transformComponent->getLocation());
	}

	for (size_t i = 1; i < components.size(); ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			bool doCollide = Collide::doCollide(std::get<0>(components[i]), std::get<1>(components[i]), std::get<0>(components[j]), std::get<1>(components[j]));

			if (doCollide)
			{
				LogInfo("test");
			}
		}
	}
}
