#pragma once

#include <string>

#include "GameData/Resources/SpriteParams.h"

struct SpriteDescription
{
	SpriteParams params;
	std::string path;
};

void to_json(nlohmann::json &outJson, const SpriteDescription &spriteDescription);
void from_json(const nlohmann::json &json, SpriteDescription &outSpriteDescription);
