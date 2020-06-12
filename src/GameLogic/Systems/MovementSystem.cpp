#include "Base/precomp.h"

#include "GameLogic/Systems/MovementSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/TrackedSpatialEntitiesComponent.generated.h"
#include "GameData/Components/SpatialTrackComponent.generated.h"

#include "GameData/World.h"


MovementSystem::MovementSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void MovementSystem::update()
{
	World& world = mWorldHolder.getWorld();
	const GameplayTimestamp timestampNow = mTime.currentTimestamp;

	struct CellScheduledTransfers
	{
		CellPos cellTo;
		EntityView entityView;

		MAYBE_UNUSED CellScheduledTransfers(CellPos to, EntityView entity)
			: cellTo(to)
			, entityView(entity)
		{
		}
	};

	std::vector<CellScheduledTransfers> transfers;

	world.getSpatialData().getAllCellManagers().forEachSpatialComponentSetWithEntity<MovementComponent, TransformComponent>([timestampNow, &transfers](Entity entity, WorldCell* cell, MovementComponent* movement, TransformComponent* transform)
	{
		EntityView entitiyView{ entity, cell->getEntityManager() };

		if (!movement->getNextStep().isZeroLength())
		{
			Vector2D pos = transform->getLocation() + movement->getNextStep();
			transform->setLocation(pos);

			CellPos cellPos = cell->getPos();
			pos.x -= cellPos.x * SpatialWorldData::CellSize;
			pos.y -= cellPos.y * SpatialWorldData::CellSize;
			bool isCellChanged = SpatialWorldData::TransformCellPos(cellPos, pos);
			if (isCellChanged)
			{
				transfers.emplace_back(cellPos, entitiyView);
			}
			transform->setUpdateTimestamp(timestampNow);
			movement->setNextStep(ZERO_VECTOR);
		}

		if (transform->getRotation() != movement->getSightDirection().rotation())
		{
			transform->setRotation(movement->getSightDirection().rotation());
			transform->setUpdateTimestamp(timestampNow);
		}
	});

	for (auto& transfer : transfers)
	{
		if (auto [spatialTracked] = transfer.entityView.getComponents<SpatialTrackComponent>(); spatialTracked != nullptr)
		{
			StringID spatialTrackID = spatialTracked->getId();
			auto [trackedComponents] = world.getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();
			auto it = trackedComponents->getEntitiesRef().find(spatialTrackID);
			if (it != trackedComponents->getEntitiesRef().end())
			{
				it->second.cell = transfer.cellTo;
			}
		}
		// the component pointer get invalidated from this line
		transfer.entityView.getManager().transferEntityTo(world.getSpatialData().getOrCreateCell(transfer.cellTo).getEntityManager(), transfer.entityView.getEntity());
	}
}
