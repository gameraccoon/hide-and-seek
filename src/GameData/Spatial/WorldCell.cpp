#include "GameData/Spatial/WorldCell.h"

#include <nlohmann/json.hpp>

nlohmann::json WorldCell::toJson(const ComponentFactory& componentFactory) const
{
	return nlohmann::json{
		{"entity_manager", mEntityManager.toJson(componentFactory)},
		{"cell_components", mCellComponents.toJson(componentFactory)},
	};
}

void WorldCell::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
	mCellComponents.fromJson(json.at("cell_components"), componentFactory);
}
