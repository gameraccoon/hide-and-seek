#pragma once

#include <optional>

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"
#include "ECS/EntityView.h"

#include "GameData/Spatial/SpatialWorldData.h"

struct ComponentSerializersHolder;

class World
{
public:
	EntityManager& getEntityManager() { return mEntityManager; }
	const EntityManager& getEntityManager() const { return mEntityManager; }

	ComponentSetHolder& getWorldComponents() { return mWorldComponents; }
	const ComponentSetHolder& getWorldComponents() const { return mWorldComponents; }

	SpatialWorldData& getSpatialData() { return mSpatialData; }
	const SpatialWorldData& getSpatialData() const { return mSpatialData; }

	nlohmann::json toJson(const ComponentSerializersHolder& componentSerializers) const;
	void fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers);

	std::optional<std::pair<EntityView, CellPos>> getTrackedSpatialEntity(StringID entityStringID);
	EntityView createTrackedSpatialEntity(StringID entityStringID, CellPos pos);
	EntityView createSpatialEntity(CellPos pos);

	void packForJsonSaving();
	void clearCaches();

private:
	EntityManager mEntityManager;
	ComponentSetHolder mWorldComponents;
	SpatialWorldData mSpatialData;
};
