#pragma once

#include "GameData/FSM/StateMachine.h"
#include "GameData/Components/AnimationComponent.generated.h"

namespace Animations
{
	// ToDo: need an editor not to hardcode SM data
	void RegisterAnimations(AnimationComponent* animationData);
}
