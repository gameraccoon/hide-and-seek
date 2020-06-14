#pragma once

#include "HAL/KeyStatesMap.h"

class InputData
{
public:
	void clearAfterFrame()
	{
		KeyboardKeyStates.clearLastFrameState();
		MouseKeyStates.clearLastFrameState();
	}

public:
	HAL::KeyStatesMap KeyboardKeyStates;
	HAL::KeyStatesMap MouseKeyStates;
	Vector2D MousePos = ZERO_VECTOR;
	Vector2D WindowSize = ZERO_VECTOR;
};
