#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"

#include "GameData/Spatial/SpatialWorldData.h"

class World
{
public:
	EntityManager& getEntityManager() { return mEntityManager; }
	ComponentSetHolder& getWorldComponents() { return mWorldComponents; }
	SpatialWorldData& getSpatialData() { return mSpatialData; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

	std::pair<OptionalEntity, EntityManager*> getSpatialEntity(StringID entityStringID);

private:
	EntityManager mEntityManager;
	ComponentSetHolder mWorldComponents;
	SpatialWorldData mSpatialData;
};
