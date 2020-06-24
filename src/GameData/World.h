#pragma once

#include <optional>

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"
#include "ECS/EntityView.h"

#include "GameData/Spatial/SpatialWorldData.h"

class ComponentSerializersHolder;

class World
{
public:
	EntityManager& getEntityManager() { return mEntityManager; }
	ComponentSetHolder& getWorldComponents() { return mWorldComponents; }
	SpatialWorldData& getSpatialData() { return mSpatialData; }

	nlohmann::json toJson(const ComponentSerializersHolder& componentSerializers) const;
	void fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers);

	std::optional<std::pair<EntityView, CellPos>> getTrackedSpatialEntity(StringID entityStringID);
	EntityView createTrackedSpatialEntity(StringID entityStringID, CellPos pos);
	EntityView createSpatialEntity(CellPos pos);

private:
	EntityManager mEntityManager;
	ComponentSetHolder mWorldComponents;
	SpatialWorldData mSpatialData;
};
