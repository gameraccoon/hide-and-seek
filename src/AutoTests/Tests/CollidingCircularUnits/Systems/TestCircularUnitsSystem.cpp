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

	std::optional<std::pair<EntityView, CellPos>> playerEntity = world.getTrackedSpatialEntity(STR_TO_ID("ControlledEntity"));
	if (!playerEntity.has_value())
	{
		return;
	}

	auto [playerTransform] = playerEntity->first.getComponents<TransformComponent>();
	if (playerTransform == nullptr)
	{
		return;
	}

	Vector2D targetLocation = playerTransform->getLocation();

	SpatialEntityManager spatialManager = world.getSpatialData().getCellManagersAround(CellPos(0, 0), Vector2D(0.0f, 0.0f), Vector2D(10000.0f, 10000.0f));
	spatialManager.forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent>([targetLocation](AiControllerComponent* /*aiController*/, TransformComponent* transform, MovementComponent* movement)
	{
		movement->setMoveDirection(targetLocation - transform->getLocation());
		movement->setSpeed(movement->getOriginalSpeed());
	});
}
