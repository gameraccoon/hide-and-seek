#include "Game/Systems/AiSystem.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/NavMeshGenerator.h"

void AiSystem::update(World* world, float dt)
{
	auto [navMeshComponent] = world->getWorldComponents().getComponents<NavMeshComponent>();

	if (navMeshComponent == nullptr)
	{
		return;
	}

	// ToDo: we recalculate navmesh every frame to be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	NavMeshGenerator generator;
	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> collisions = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();
	generator.generateNavMesh(navMeshComponent->getNavMeshRef(), collisions);

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
	world->getEntityManger().forEachComponentSet<AiControllerComponent, TransformComponent, MovementComponent>([playerTransform, speed, dt](AiControllerComponent*, TransformComponent* transform, MovementComponent* movement)
	{
		Vector2D location = transform->getLocation();
		Vector2D diff = playerTransform->getLocation() - location;
		Vector2D vectorSpeed = diff.ort() * speed;
		movement->setSpeed(vectorSpeed);
		transform->setLocation(location + vectorSpeed * dt);
	});
}
