#pragma once

#include <string>

#include <nlohmann/json_fwd.hpp>

struct AnimationParams
{
	float speed = 1.0f;
	bool isLooped = false;
	std::string globalAnimId; // a way to find FSM
};

void to_json(nlohmann::json &outJson, const AnimationParams &animationParams);
void from_json(const nlohmann::json &json, AnimationParams &outAnimationParams);
