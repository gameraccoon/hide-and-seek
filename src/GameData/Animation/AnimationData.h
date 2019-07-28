#pragma once

#include <GameData/FSM/StateMachine.h>

template <typename StateIDType, typename BlackboardKeyType>
class AnimationData
{
public:
	const FSM::StateMachine<StateIDType, BlackboardKeyType>* BlackboardPtr;
	StateIDType CurrentState;
	float startTime;
};
