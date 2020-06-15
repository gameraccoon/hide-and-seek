#include "Base/precomp.h"

#include "GameLogic/Systems/WeaponSystem.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "GameData/Components/WeaponComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Spatial/SpatialEntity.h"

#include "Utils/Geometry/RayTrace.h"


WeaponSystem::WeaponSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

struct ShotInfo
{
	Entity instigator;
	WorldCell* instigatorCell;
};

void WeaponSystem::update()
{
	World& world = mWorldHolder.getWorld();
	GameplayTimestamp currentTime = mTime.currentTimestamp;

	std::vector<ShotInfo> shotsToMake;

	// for tests
	float traceSize = 500.0f;

	world.getSpatialData().getAllCellManagers().forEachSpatialComponentSetWithEntity<WeaponComponent, CharacterStateComponent>([currentTime, &shotsToMake](Entity entity, WorldCell* cell, WeaponComponent* weaponComponent, CharacterStateComponent* characterState)
	{
		if (characterState->getState() == CharacterState::Shoot || characterState->getState() == CharacterState::WalkAndShoot)
		{
			if (currentTime > weaponComponent->getShotFinishTimestamp())
			{
				shotsToMake.push_back({entity, cell});

				weaponComponent->setShotFinishTimestamp(currentTime.getIncreasedByFloatTime(weaponComponent->getShotPeriod()));
			}
		}
	});

	for (auto& shotInfo : shotsToMake)
	{
		auto [transform] = shotInfo.instigatorCell->getEntityManager().getEntityComponents<TransformComponent>(shotInfo.instigator);
		if (transform)
		{
			RayTrace::TraceResult result = RayTrace::Trace(
				world,
				transform->getLocation(),
				transform->getLocation() + Vector2D(transform->getRotation()) * traceSize
			);

			if (result.hasHit)
			{
				WorldCell* cell = world.getSpatialData().getCell(result.hitEntity.cell);
				AssertFatal(cell != nullptr, "Cell of the hit object is not found");
				cell->getEntityManager().removeEntity(result.hitEntity.entity.getEntity());
			}
		}
	}
}
