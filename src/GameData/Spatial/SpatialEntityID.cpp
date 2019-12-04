#include "GameData/Spatial/SpatialEntityID.h"

#include <nlohmann/json.hpp>

SpatialEntityID::SpatialEntityID(Entity entity, CellPos cellPos)
	: entity(entity)
	, cell(cellPos)
{
}

bool SpatialEntityID::operator==(const SpatialEntityID& other) const
{
	return cell == other.cell && entity.mId == other.entity.mId;
}

bool SpatialEntityID::operator!=(const SpatialEntityID& other) const
{
	return !(*this == other);
}

void to_json(nlohmann::json& outJson, const SpatialEntityID& spatialEntityID)
{
	outJson = nlohmann::json{
		{"cell", spatialEntityID.cell},
		{"entity", spatialEntityID.entity}
	};
}

void from_json(const nlohmann::json& json, SpatialEntityID& outSpatialEntityID)
{
	json.at("cell").get_to(outSpatialEntityID.cell);
	json.at("entity").get_to(outSpatialEntityID.entity);
}
