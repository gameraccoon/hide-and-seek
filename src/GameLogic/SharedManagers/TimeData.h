#pragma once

#include "GameData/Core/GameTime.h"

class TimeData
{
public:
	TimeData();
	void update(float dt);

public:
	static constexpr int cycleLength = 3600;
	static constexpr int cycleLengthMax = 14400;
	static_assert (cycleLengthMax % cycleLength == 0, "cycleLengthMax should be dividable by cycleLength");
	static constexpr int parallelCyclesCount = cycleLengthMax / cycleLength;

	float dt = 0.0f;
	GameTime::CycleIdxType currentCycle = 0;
	GameTime::CycleIdxType cyclesPassed = 0;
	GameTime::CycleTimeType cycleTime[parallelCyclesCount];
};
