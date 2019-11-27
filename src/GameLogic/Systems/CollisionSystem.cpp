#include "GameLogic/Systems/CollisionSystem.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/Collide.h"


CollisionSystem::CollisionSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void CollisionSystem::update()
{
	World& world = mWorldHolder.getWorld();
	const GameplayTimestamp timestampNow = mTime.currentTimestamp;

	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> components;
	world.getEntityManager().getComponents<CollisionComponent, TransformComponent>(components);

	for (auto& [collision, transform] : components)
	{
		if (collision->getIsBoundingBoxDirty())
		{
			Collide::UpdateOriginalBoundingBox(collision);
		}

		collision->setBoundingBox(collision->getOriginalBoundingBox() + transform->getLocation());
	}

	world.getEntityManager().forEachComponentSet<CollisionComponent, TransformComponent, MovementComponent>([&components, timestampNow](CollisionComponent* collisionComponent, TransformComponent* transformComponent, MovementComponent* /*movementComponent*/)
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
						transformComponent->setUpdateTimestamp(timestampNow);
					}
					else
					{
						transformComponent->setLocation(transformComponent->getLocation() + resist*0.5f);
						transform->setLocation(transform->getLocation() - resist*0.5f);
						transform->setUpdateTimestamp(timestampNow);
					}
				}
			}
		}
	});
}
