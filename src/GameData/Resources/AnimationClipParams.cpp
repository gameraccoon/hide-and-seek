#include "GameData/Resources/AnimationClipParams.h"

#include <nlohmann/json.hpp>

void to_json(nlohmann::json& outJson, const AnimationClipParams& animationParams)
{
	outJson = nlohmann::json::object({
		{"globalAnimId", animationParams.globalAnimId},
		{"isLooped", animationParams.isLooped},
		{"speed", animationParams.speed}
	});
}

void from_json(const nlohmann::json& json, AnimationClipParams& outAnimationParams)
{
	json.at("globalAnimId").get_to(outAnimationParams.globalAnimId);
	json.at("isLooped").get_to(outAnimationParams.isLooped);
	json.at("speed").get_to(outAnimationParams.speed);
}
