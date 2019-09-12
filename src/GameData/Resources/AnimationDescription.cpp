#include "GameData/Resources/AnimationDescription.h"

#include <nlohmann/json.hpp>

void to_json(nlohmann::json &outJson, const AnimationDescription &animationDescription)
{
	outJson = nlohmann::json::object({
		{"params", animationDescription.params},
		{"spriteParams", animationDescription.spriteParams},
		{"path", animationDescription.path}
	});
}

void from_json(const nlohmann::json &json, AnimationDescription &outAnimationDescription)
{
	json.at("params").get_to(outAnimationDescription.params);
	json.at("spriteParams").get_to(outAnimationDescription.spriteParams);
	json.at("path").get_to(outAnimationDescription.path);
}
