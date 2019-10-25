#pragma once

#include <map>
#include <string>

#include "GameData/Core/ResourceHandle.h"
#include "GameData/FSM/Blackboard.h"

template <typename StateIDType>
class AnimationGroup
{
public:
	StateIDType currentState;
	std::map<StateIDType, ResourceHandle> animations;
	std::string stateMachineId;
	size_t animationIdx;
};
