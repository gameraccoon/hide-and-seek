#pragma once

#include "GameData/Core/Vector2D.h"

#include "HAL/KeyStatesMap.h"

class InputData
{
public:
	void clearAfterFrame()
	{
		keyboardKeyStates.clearLastFrameState();
		mouseKeyStates.clearLastFrameState();
	}

public:
	HAL::KeyStatesMap keyboardKeyStates;
	HAL::KeyStatesMap mouseKeyStates;
	Vector2D mousePos = ZERO_VECTOR;
	Vector2D windowSize = ZERO_VECTOR;
};
