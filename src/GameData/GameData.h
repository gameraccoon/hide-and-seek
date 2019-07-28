#pragma once

#include <nlohmann/json_fwd.hpp>

#include "ECS/ComponentSetHolder.h"

class GameData
{
public:
	ComponentSetHolder& getGameComponents() { return mGameComponents; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	ComponentSetHolder mGameComponents;
};
