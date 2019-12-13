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

	world.getSpatialData().getAllCellManagers().forEachSpatialComponentSetWithEntity<MovementComponent, TransformComponent>([timestampNow, &world, &transfers](WorldCell* cell, EntityView entitiyView, MovementComponent* movement, TransformComponent* transform)
	{
		if (!movement->getNextStep().isZeroLength())
		{
			CellPos cellPos = cell->getPos();
			Vector2D pos = transform->getLocation() + movement->getNextStep();
			bool isCellChanged = SpatialWorldData::TransformCellPos(cellPos, pos);
			if (isCellChanged)
			{
				if (auto [spatialTracked] = entitiyView.getComponents<SpatialTrackComponent>(); spatialTracked != nullptr)
				{
					StringID spatialTrackID = spatialTracked->getId();
					auto [trackedComponents] = world.getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();
					auto it = trackedComponents->getEntitiesRef().find(spatialTrackID);
					if (it != trackedComponents->getEntitiesRef().end())
					{
						it->second.cell = cellPos;
					}
				}
				transfers.emplace_back(cellPos, entitiyView);
			}
			transform->setLocation(pos);
			transform->setUpdateTimestamp(timestampNow);
		}

		if (transform->getRotation() != movement->getSightDirection().rotation())
		{
			transform->setRotation(movement->getSightDirection().rotation());
			transform->setUpdateTimestamp(timestampNow);
		}
	});

	for (auto& transfer : transfers)
	{
		transfer.entityView.getManager().transferEntityTo(world.getSpatialData().getOrCreateCell(transfer.cellTo).getEntityManager(), transfer.entityView.getEntity());
	}
}
