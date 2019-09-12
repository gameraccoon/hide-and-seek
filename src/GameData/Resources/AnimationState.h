#pragma once

#include <string>

#include "GameData/Core/ResourceHandle.h"

template <typename StateIDType>
class AnimationState
{
public:
	StateIDType currentState;
	float progress = 0.0f; // [0;1]
	ResourceHandle animHandle;
	std::string animStateId;
};
