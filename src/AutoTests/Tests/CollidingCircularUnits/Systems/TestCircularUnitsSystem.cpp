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

	std::optional<EntityView> playerEntity = world.getTrackedSpatialEntity(STR_TO_ID("ControlledEntity"));
	if (!playerEntity.has_value())
	{
		return;
	}

	auto [playerTransform] = playerEntity->getComponents<TransformComponent>();
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
