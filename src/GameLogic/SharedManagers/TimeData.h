#pragma once

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
	int currentCycle = 0;
	int cyclesPassed = 0;
	float cycleTime[parallelCyclesCount];
};
