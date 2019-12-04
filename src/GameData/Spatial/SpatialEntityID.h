#pragma once

#include <nlohmann/json_fwd.hpp>

#include "ECS/Entity.h"

#include "GameData/Spatial/CellPos.h"

struct SpatialEntityID
{
	OptionalEntity entity;
	CellPos cell;

	SpatialEntityID() = default;
	SpatialEntityID(Entity entity, CellPos cellPos);

	bool operator==(const SpatialEntityID& other) const;
	bool operator!=(const SpatialEntityID& other) const;

	friend void to_json(nlohmann::json& outJson, const SpatialEntityID& spatialEntityID);
	friend void from_json(const nlohmann::json& json, SpatialEntityID& outSpatialEntityID);
};
