#pragma once

#include <vector>

#include "GameData/Core/ResourceHandle.h"
#include "GameData/Resources/AnimationParams.h"

struct AnimationData
{
	std::vector<ResourceHandle> sprites;
	AnimationParams params;
	float progress = 0;
	int spriteId;
	ResourceHandle animation;
};
