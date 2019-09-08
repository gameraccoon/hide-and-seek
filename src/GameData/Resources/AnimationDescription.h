#pragma once

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "GameData/Resources/AnimationParams.h"
#include "GameData/Resources/SpriteParams.h"

struct AnimationDescription
{
	AnimationParams params;
	SpriteParams spriteParams;
	std::string path;
};

void to_json(nlohmann::json& outJson, const AnimationDescription& animationDescription);
void from_json(const nlohmann::json& json, AnimationDescription& outAnimationDescription);
