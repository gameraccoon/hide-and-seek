#pragma once

#include <string>

#include <nlohmann/json_fwd.hpp>

struct AnimationParams
{
	float speed = 1.0f;
	bool isLooped = false;
	std::string globalAnimId; // a way to find FSM
};

inline bool operator ==(const AnimationParams &a, const AnimationParams &b)
{
	return a.speed == b.speed && a.isLooped == b.isLooped && a.globalAnimId == b.globalAnimId;
}

inline bool operator !=(const AnimationParams &a, const AnimationParams &b)
{
	return !(a == b);
}

void to_json(nlohmann::json &outJson, const AnimationParams &animationParams);
void from_json(const nlohmann::json &json, AnimationParams &outAnimationParams);
