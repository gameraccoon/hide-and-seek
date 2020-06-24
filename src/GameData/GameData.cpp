#include "Base/precomp.h"

#include "GameData/GameData.h"

#include <nlohmann/json.hpp>

nlohmann::json GameData::toJson(const ComponentSerializersHolder& componentSerializers) const
{
	return nlohmann::json{
		{"game_components", mGameComponents.toJson(componentSerializers)}
	};
}

void GameData::fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers)
{
	mGameComponents.fromJson(json.at("game_components"), componentSerializers);
}
