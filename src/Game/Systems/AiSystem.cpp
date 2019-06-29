#include "Game/Systems/AiSystem.h"

#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/NavMeshGenerator.h"

void AiSystem::update(World* world, float /*dt*/)
{
	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> collisions = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();

	// ToDo: we recalculate navmesh every frame to be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	NavMeshGenerator generator;
	auto [navMeshComponent] = world->getWorldComponents().getComponents<NavMeshComponent>();

	if (navMeshComponent == nullptr)
	{
		return;
	}

	generator.generateNavMesh(navMeshComponent->getNavMeshRef(), collisions);

	world->getEntityManger().forEachComponentSet<AiControllerComponent>([](AiControllerComponent* /*aiController*/)
	{

	});
}
