#pragma once

#include "ECS/EntityView.h"

#include "GameData/Spatial/SpatialEntity.h"

#include "entityreference.h"

class World;

namespace Utils
{
	std::optional<EntityView> GetEntityView(const EntityReference& reference, World* world);
	std::optional<EntityView> GetSpatialEntityView(const SpatialEntity& spatialEntity, World* world);
}
