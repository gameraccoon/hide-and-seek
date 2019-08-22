#pragma once

#include "GameData/FSM/StateMachine.h"
#include "GameData/Components/StateMachineComponent.generated.h"

namespace StateMachines
{
	// ToDo: need an editor not to hardcode SM data
	void RegisterStateMachines(StateMachineComponent* stateMachine);
}
