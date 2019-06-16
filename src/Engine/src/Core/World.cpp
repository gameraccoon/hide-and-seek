#include "Core/World.h"

#include <nlohmann/json.hpp>

nlohmann::json World::toJson(const ComponentFactory& componentFactory) const
{
	return nlohmann::json{
		{"entity_manager", mEntityManager.toJson(componentFactory)},
		{"world_components", mWorldComponents.toJson(componentFactory)},
		{"player_controlled_entity", mPlayerControlledEntity},
		{"main_camera", mMainCamera}
	};
}

void World::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
	mWorldComponents.fromJson(json.at("world_components"), componentFactory);
	json.at("player_controlled_entity").get_to(mPlayerControlledEntity);
	json.at("main_camera").get_to(mMainCamera);
}
