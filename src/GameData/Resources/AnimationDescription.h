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

inline bool operator ==(const AnimationDescription &a, const AnimationDescription &b)
{
	return a.params == b.params && a.spriteParams == b.spriteParams && a.path == b.path;
}

inline bool operator !=(const AnimationDescription &a, const AnimationDescription &b)
{
	return !(a == b);
}

void to_json(nlohmann::json& outJson, const AnimationDescription& animationDescription);
void from_json(const nlohmann::json& json, AnimationDescription& outAnimationDescription);
