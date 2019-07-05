#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestCircularUnitsSystem.h"

#include "GameData/World.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"

void TestCircularUnitsSystem::update(World* world, float dt)
{
	OptionalEntity playerEntity = world->getPlayerControlledEntity();
	if (!playerEntity.isValid())
	{
		return;
	}

	auto [playerTransform] = world->getEntityManger().getEntityComponents<TransformComponent>(playerEntity.getEntity());
	if (playerTransform == nullptr)
	{
		return;
	}

	float speed = 30.0f;
	Vector2D targetLocation = playerTransform->getLocation();

	world->getEntityManger().forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent>([targetLocation, speed, dt](AiControllerComponent* /*aiController*/, TransformComponent* transform, MovementComponent* movement)
	{
		Vector2D startLocation = transform->getLocation();
		Vector2D diff = targetLocation - startLocation;
		Vector2D vectorSpeed = diff.ort() * speed;
		movement->setSpeed(vectorSpeed);
		transform->setLocation(startLocation + vectorSpeed * dt);
	});
}
