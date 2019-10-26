#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestCircularUnitsSystem.h"

#include "GameData/World.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"


TestCircularUnitsSystem::TestCircularUnitsSystem(WorldHolder& worldHolder)
	: mWorldHolder(worldHolder)
{
}

void TestCircularUnitsSystem::update()
{
	World& world = mWorldHolder.getWorld();

	OptionalEntity playerEntity = world.getPlayerControlledEntity();
	if (!playerEntity.isValid())
	{
		return;
	}

	auto [playerTransform] = world.getEntityManager().getEntityComponents<TransformComponent>(playerEntity.getEntity());
	if (playerTransform == nullptr)
	{
		return;
	}

	Vector2D targetLocation = playerTransform->getLocation();

	world.getEntityManager().forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent>([targetLocation](AiControllerComponent* /*aiController*/, TransformComponent* transform, MovementComponent* movement)
	{
		movement->setMoveDirection(targetLocation - transform->getLocation());
		movement->setSpeed(movement->getOriginalSpeed());
	});
}
