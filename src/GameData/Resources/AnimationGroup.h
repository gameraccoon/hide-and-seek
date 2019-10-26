#pragma once

#include <map>
#include <vector>
#include <string>

#include "GameData/Core/ResourceHandle.h"
#include "GameData/FSM/Blackboard.h"

template <typename StateIDType>
class AnimationGroup
{
public:
	StateIDType currentState;
	std::map<StateIDType, std::vector<ResourceHandle>> animationClips;
	std::string stateMachineId;
	size_t animationClipIdx = 0;
};
