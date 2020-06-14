#pragma once

#include "GameData/Time/GameplayTimestamp.h"

class TimeData
{
public:
	void update(float dt);

public:
	float dt = 0.0f;
	GameplayTimestamp currentTimestamp{0};
};
