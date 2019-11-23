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

std::pair<OptionalEntity, EntityManager*> World::getSpatialEntity(StringID entityStringID)
{
	EntityManager* playerCellEntityManager = nullptr;
	OptionalEntity playerEntity;

	auto [trackedSpatialEntities] = getWorldComponents().getComponents<TrackedSpatialEntitiesComponent>();

	if (trackedSpatialEntities)
	{
		auto it = trackedSpatialEntities->getEntities().find(entityStringID);
		if (it != trackedSpatialEntities->getEntities().end())
		{
			if (WorldCell* cell = getSpatialData().getCell(it->second.cell))
			{
				playerCellEntityManager = &cell->getEntityManager();
				playerEntity = it->second.entity;
			}
		}
	}

	return std::make_pair(playerEntity, playerCellEntityManager);
}
