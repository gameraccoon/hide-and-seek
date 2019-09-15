#include "GameData/GameData.h"

#include <nlohmann/json.hpp>

nlohmann::json GameData::toJson(const ComponentFactory& componentFactory) const
{
	return nlohmann::json{
		{"game_components", mGameComponents.toJson(componentFactory)}
	};
}

void GameData::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	mGameComponents.fromJson(json.at("game_components"), componentFactory);
}
