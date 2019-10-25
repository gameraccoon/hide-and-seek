#include "GameLogic/Systems/CollisionSystem.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/Collide.h"


CollisionSystem::CollisionSystem(WorldHolder& worldHolder)
	: mWorldHolder(worldHolder)
{
}

void CollisionSystem::update()
{
	World* world = mWorldHolder.world;

	auto components = world->getEntityManager().getComponents<CollisionComponent, TransformComponent>();

	for (auto& [collision, transform] : components)
	{
		if (collision->getIsBoundingBoxDirty())
		{
			Collide::UpdateOriginalBoundingBox(collision);
		}

		collision->setBoundingBox(collision->getOriginalBoundingBox() + transform->getLocation());
	}

	world->getEntityManager().forEachComponentSet<CollisionComponent, TransformComponent, MovementComponent>([&components](CollisionComponent* collisionComponent, TransformComponent* transformComponent, MovementComponent* /*movementComponent*/)
	{
		Vector2D resist = ZERO_VECTOR;
		for (auto& [collision, transform] : components)
		{
			if (collision != collisionComponent)
			{
				bool doCollide = Collide::DoCollide(collisionComponent, transformComponent->getLocation(), collision, transform->getLocation(), resist);

				if (doCollide)
				{
					if (collision->getGeometry().type == HullType::Angular)
					{
						transformComponent->setLocation(transformComponent->getLocation() + resist);
					}
					else
					{
						transformComponent->setLocation(transformComponent->getLocation() + resist*0.5f);
						transform->setLocation(transform->getLocation() - resist*0.5f);
					}
				}
			}
		}
	});
}
