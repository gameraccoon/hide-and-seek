#include "GameData/World.h"

#include <nlohmann/json.hpp>

#include "GameData/Components/TrackedSpatialEntitiesComponent.generated.h"

nlohmann::json World::toJson(const ComponentFactory& componentFactory) const
{
	return nlohmann::json{
		{"entity_manager", mEntityManager.toJson(componentFactory)},
		{"world_components", mWorldComponents.toJson(componentFactory)},
		{"spatial_data", mSpatialData.toJson(componentFactory)}
	};
}

void World::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
	mWorldComponents.fromJson(json.at("world_components"), componentFactory);
	mSpatialData.fromJson(json.at("spatial_data"), componentFactory);
}

std::optional<EntityView> World::getTrackedSpatialEntity(StringID entityStringID)
{
	auto [trackedSpatialEntities] = getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();

	if (trackedSpatialEntities)
	{
		auto it = trackedSpatialEntities->getEntities().find(entityStringID);
		if (it != trackedSpatialEntities->getEntities().end())
		{
			if (WorldCell* cell = getSpatialData().getCell(it->second.cell))
			{
				return EntityView(it->second.entity, cell->getEntityManager());
			}
		}
	}

	return std::nullopt;
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
	return result;
}

EntityView World::createSpatialEntity(CellPos pos)
{
	WorldCell& cell = getSpatialData().getOrCreateCell(pos);
	return EntityView(cell.getEntityManager().addEntity(), cell.getEntityManager());
}
