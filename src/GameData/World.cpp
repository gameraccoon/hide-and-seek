#include "GameData/World.h"

#include <nlohmann/json.hpp>

#include "GameData/Components/TrackedSpatialEntitiesComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/SpatialTrackComponent.generated.h"

nlohmann::json World::toJson(const ComponentFactory& componentFactory) const
{
	ReportError("This function not fully mirrors 'fromJson', it's not safe to use it right now");

	return nlohmann::json{
		{"entity_manager", mEntityManager.toJson(componentFactory)},
		{"world_components", mWorldComponents.toJson(componentFactory)},
		{"spatial_data", mSpatialData.toJson(componentFactory)}
	};
}

static void InitSpatialTracked(SpatialTrackComponent* spatialTracked, const CellPos& cellPos, ComponentSetHolder& worldComponents)
{
	StringID spatialTrackID = spatialTracked->getId();
	auto [trackedComponents] = worldComponents.getComponents<TrackedSpatialEntitiesComponent>();
	auto it = trackedComponents->getEntitiesRef().find(spatialTrackID);
	if (it != trackedComponents->getEntitiesRef().end())
	{
		it->second.cell = cellPos;
	}
}

static void DistributeSpatialEntitiesBetweenCells(EntityManager& entityManager, ComponentSetHolder& worldComponents, SpatialWorldData& spatialData)
{
	auto [trackedSpatialEntities] = worldComponents.getComponents<TrackedSpatialEntitiesComponent>();
	for (auto entityPair : trackedSpatialEntities->getEntitiesRef())
	{
		SpatialTrackComponent* spatialTrack = entityManager.addComponent<SpatialTrackComponent>(entityPair.second.entity.getEntity());
		spatialTrack->setId(entityPair.first);
	}

	std::vector<Entity> spatialEntities = entityManager.getEntitiesHavingComponents<TransformComponent>();
	for (Entity entity : spatialEntities)
	{
		auto [transform] = entityManager.getEntityComponents<TransformComponent>(entity);
		Vector2D pos = transform->getLocation();
		CellPos cellPos(0, 0);
		SpatialWorldData::TransformCellPos(cellPos, pos);
		transform->setLocation(pos);
		WorldCell& cell = spatialData.getOrCreateCell(cellPos);
		entityManager.transferEntityTo(cell.getEntityManager(), entity);

		if (auto [spatialTracked] = cell.getEntityManager().getEntityComponents<SpatialTrackComponent>(entity); spatialTracked != nullptr)
		{
			InitSpatialTracked(spatialTracked, cellPos, worldComponents);
		}
	}
}

void World::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
	mWorldComponents.fromJson(json.at("world_components"), componentFactory);
	mSpatialData.fromJson(json.at("spatial_data"), componentFactory);

	DistributeSpatialEntitiesBetweenCells(mEntityManager, mWorldComponents, mSpatialData);
}

std::optional<std::pair<EntityView, CellPos>> World::getTrackedSpatialEntity(StringID entityStringID)
{
	std::optional<std::pair<EntityView, CellPos>> result;
	auto [trackedSpatialEntities] = getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();

	if (trackedSpatialEntities)
	{
		auto it = trackedSpatialEntities->getEntities().find(entityStringID);
		if (it != trackedSpatialEntities->getEntities().end())
		{
			if (WorldCell* cell = getSpatialData().getCell(it->second.cell))
			{
				result.emplace(EntityView(it->second.entity.getEntity(), cell->getEntityManager()), cell->getPos());
			}
		}
	}

	return result;
}

EntityView World::createTrackedSpatialEntity(StringID entityStringID, CellPos pos)
{
	auto result = createSpatialEntity(pos);
	auto [trackedSpatialEntities] = getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();
	if (trackedSpatialEntities == nullptr)
	{
		trackedSpatialEntities = getWorldComponents().addComponent<TrackedSpatialEntitiesComponent>();
	}
	trackedSpatialEntities->getEntitiesRef().insert_or_assign(entityStringID, SpatialEntityID(result.getEntity(), pos));
	SpatialTrackComponent* trackComponent = result.addComponent<SpatialTrackComponent>();
	trackComponent->setId(entityStringID);
	return result;
}

EntityView World::createSpatialEntity(CellPos pos)
{
	WorldCell& cell = getSpatialData().getOrCreateCell(pos);
	return EntityView(cell.getEntityManager().addEntity(), cell.getEntityManager());
}
