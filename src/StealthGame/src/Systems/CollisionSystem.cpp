#include "Systems/CollisionSystem.h"

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>
#include <Components/MovementComponent.h>
#include <Modules/Collide.h>

void CollisionSystem::update(World* world, float /*dt*/)
{
	auto components = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();

	for (auto& [collision, transform] : components)
	{
		if (collision->isBoundingBoxDirty())
		{
			Collide::updateOriginalBoundingBox(collision);
		}

		collision->setBoundingBox(collision->getOriginalBoundingBox() + transform->getLocation());
	}

	world->getEntityManger().forEachEntity<CollisionComponent, TransformComponent, MovementComponent>([&components](CollisionComponent* collisionComponent, TransformComponent* transformComponent, MovementComponent* /*movementComponent*/)
	{
		Vector2D resist = ZERO_VECTOR;
		for (auto& [collision, transform] : components)
		{
			if (collision != collisionComponent)
			{
				bool doCollide = Collide::doCollide(collisionComponent, transformComponent->getLocation(), collision, transform->getLocation(), resist);

				if (doCollide)
				{
					transformComponent->shiftLocation(resist);
				}
			}
		}
	});
}
