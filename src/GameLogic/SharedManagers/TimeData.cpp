#include "GameLogic/SharedManagers/TimeData.h"

TimeData::TimeData()
{
	for (int i = 0; i < parallelCyclesCount; ++i)
	{
		cycleTime[i] = 0.0f;
	}
}

void TimeData::update(float dt)
{
	this->dt = dt;

	for (int i = 0; i < parallelCyclesCount; ++i)
	{
		cycleTime[i] += dt;
	}

	if (cycleTime[currentCycle] > cycleLength)
	{
		currentCycle = (currentCycle + 1) % parallelCyclesCount;
		cycleTime[currentCycle] -= cycleLength;
		++cyclesPassed;
	}
}
