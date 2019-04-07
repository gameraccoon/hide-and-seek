#include "Core/World.h"

#include <nlohmann/json.hpp>

void to_json(nlohmann::json& outJson, const World& world)
{
	outJson = nlohmann::json{
		{"entity_manager", world.mEntityManager},
		{"player_controlled_entity", world.mPlayerControlledEntity},
		{"main_camera", world.mMainCamera}
	};
}

void from_json(const nlohmann::json& json, World& outWorld)
{
	json.at("entity_manager").get_to(outWorld);
	json.at("player_controlled_entity").get_to(outWorld.mPlayerControlledEntity);
	json.at("main_camera").get_to(outWorld.mMainCamera);
}
