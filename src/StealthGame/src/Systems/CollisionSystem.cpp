#include "Systems/CollisionSystem.h"

#include <Components/CollisionComponent.generated.h>
#include <Components/TransformComponent.generated.h>
#include <Components/MovementComponent.generated.h>
#include <Utils/Collide.h>
#include <Core/World.h>

void CollisionSystem::update(World* world, float /*dt*/)
{
	auto components = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();

	for (auto& [collision, transform] : components)
	{
		if (collision->getIsBoundingBoxDirty())
		{
			Collide::UpdateOriginalBoundingBox(collision);
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
				bool doCollide = Collide::DoCollide(collisionComponent, transformComponent->getLocation(), collision, transform->getLocation(), resist);

				if (doCollide)
				{
					transformComponent->setLocation(transformComponent->getLocation() + resist);
				}
			}
		}
	});
}
