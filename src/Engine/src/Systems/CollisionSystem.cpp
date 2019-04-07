#include "Systems/CollisionSystem.h"

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <Components/MovementComponent.h>
#include <Modules/Collide.h>

void CollisionSystem::update(World* world, float /*dt*/)
{
	auto components = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();

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

	world->getEntityManger().forEachEntity<CollisionComponent, TransformComponent, MovementComponent>([&components](CollisionComponent* collisionComponent, TransformComponent* transformComponent, MovementComponent* /*movementComponent*/)
	{
		Vector2D resist = ZERO_VECTOR;
		for (auto& component : components)
		{
			if (std::get<0>(component) != collisionComponent)
			{
				bool doCollide = Collide::doCollide(collisionComponent, transformComponent->getLocation(), std::get<0>(component), std::get<1>(component)->getLocation(), resist);

				if (doCollide)
				{
					transformComponent->shiftLocation(resist);
				}
			}
		}
	});
}
