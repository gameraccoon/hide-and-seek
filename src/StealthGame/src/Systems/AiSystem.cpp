#include "Systems/AiSystem.h"

#include <Components/NavMeshComponent.generated.h>
#include <Components/AiControllerComponent.generated.h>
#include <Components/CollisionComponent.generated.h>
#include <Components/TransformComponent.generated.h>

#include <Core/World.h>
#include <Utils/Geometry/NavMeshGenerator.h>

void AiSystem::update(World* world, float /*dt*/)
{
	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> collisions = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();

	// ToDo: we recalculate navmesh every frame to be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	NavMeshGenerator generator;
	std::vector<std::tuple<NavMeshComponent*>> navMeshComponents = world->getEntityManger().getComponents<NavMeshComponent>();
	for (auto [navMeshComponent] : navMeshComponents)
	{
		generator.generateNavMesh(navMeshComponent->getNavMeshRef(), collisions);
	};

	if (navMeshComponents.size() > 0)
	{
		world->getEntityManger().forEachEntity<AiControllerComponent>([](AiControllerComponent* /*aiController*/)
		{

		});
	}
}
