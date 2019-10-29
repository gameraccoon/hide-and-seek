#pragma once

#include <map>
#include <vector>
#include <string>

#include "Base/String/StringID.h"

#include "GameData/Core/ResourceHandle.h"
#include "GameData/FSM/Blackboard.h"

template <typename StateIDType>
class AnimationGroup
{
public:
	StateIDType currentState;
	std::map<StateIDType, std::vector<ResourceHandle>> animationClips;
	StringID stateMachineId;
	size_t animationClipIdx = 0;
};
