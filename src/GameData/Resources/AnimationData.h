#pragma once

#include <set>

#include "GameData/FSM/StateMachine.h"
#include "GameData/Core/ResourceHandle.h"

template <typename StateIDType>
class AnimationData
{
public:
	struct AnimData
	{
		std::string globalAnimId;
		ResourceHandle animation;
		float speed = 1.0f;
		bool isLooped = false;
	};

public:
	std::set<std::pair<StateIDType, StateIDType>> blendedStates;
	std::map<StateIDType, AnimData> animDatas;
};
