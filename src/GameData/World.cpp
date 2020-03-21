#include "Base/precomp.h"

#include "GameData/World.h"

#include <nlohmann/json.hpp>

#include "GameData/Components/TrackedSpatialEntitiesComponent.generated.h"
#include "GameData/Components/SpatialTrackComponent.generated.h"

nlohmann::json World::toJson(const ComponentFactory& componentFactory) const
{
	return nlohmann::json{
		{"entity_manager", mEntityManager.toJson(componentFactory)},
		{"world_components", mWorldComponents.toJson(componentFactory)},
		{"spatial_data", mSpatialData.toJson(componentFactory)}
	};
}

static void InitSpatialTrackedEntities(SpatialWorldData& spatialData, ComponentSetHolder& worldComponents)
{
	auto [trackedSpatialEntities] = worldComponents.getComponents<TrackedSpatialEntitiesComponent>();

	spatialData.getAllCellManagers().forEachSpatialComponentSet<SpatialTrackComponent>([trackedSpatialEntities](WorldCell* cell, SpatialTrackComponent* spatialTrack)
	{
		auto it = trackedSpatialEntities->getEntitiesRef().find(spatialTrack->getId());
		if (it != trackedSpatialEntities->getEntitiesRef().end())
		{
			it->second.cell = cell->getPos();
		}
		//else
		{
			ReportError("No tracked spatial entity record found for entity %s", spatialTrack->getId());
		}
	});
}

void World::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
	mWorldComponents.fromJson(json.at("world_components"), componentFactory);
	mSpatialData.fromJson(json.at("spatial_data"), componentFactory);

	InitSpatialTrackedEntities(mSpatialData, mWorldComponents);
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
	trackedSpatialEntities->getEntitiesRef().insert_or_assign(entityStringID, SpatialEntity(result.getEntity(), pos));
	SpatialTrackComponent* trackComponent = result.addComponent<SpatialTrackComponent>();
	trackComponent->setId(entityStringID);
	return result;
}

EntityView World::createSpatialEntity(CellPos pos)
{
	WorldCell& cell = getSpatialData().getOrCreateCell(pos);
	return EntityView(cell.getEntityManager().addEntity(), cell.getEntityManager());
}
