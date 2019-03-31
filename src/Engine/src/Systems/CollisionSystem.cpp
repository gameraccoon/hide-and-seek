#include "Systems/CollisionSystem.h"

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <Modules/Collide.h>

void CollisionSystem::update(World* world, float /*dt*/)
{
	auto components = world->getComponents<CollisionComponent, TransformComponent>();

	for (auto component : components)
	{
		auto& collisionComponent = std::get<0>(component);
		auto& transformComponent = std::get<1>(component);

		if (collisionComponent->isBoundingBoxDirty())
		{
			Collide::updateOriginalBoundingBox(collisionComponent);
		}

		collisionComponent->setBoundingBox(collisionComponent->getOriginalBoundingBox() + transformComponent->getLocation());
	}

	Vector2D resist = ZERO_VECTOR;
	for (size_t i = 1; i < components.size(); ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			bool doCollide = Collide::doCollide(std::get<0>(components[i]), std::get<1>(components[i]), std::get<0>(components[j]), std::get<1>(components[j]), resist);

			if (doCollide)
			{
				std::get<1>(components[i])->setLocation(std::get<1>(components[i])->getLocation() + resist * 0.5f);
				std::get<1>(components[j])->setLocation(std::get<1>(components[j])->getLocation() - resist * 0.5f);
			}
		}
	}
}
