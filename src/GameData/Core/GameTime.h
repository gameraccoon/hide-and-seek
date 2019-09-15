#pragma once

/**
 * Helps to calculate differences in game time
 *
 * Should be used only for short periods of time (less than 1 hr)
 *
 * Isn't initialized by default
 */
struct GameTime
{
	using CycleIdxType = int;
	using CycleTimeType = float;

	CycleIdxType cycleIndex;
	CycleTimeType cycleTime;

	GameTime() = default;
	GameTime(CycleIdxType cycleIndex, CycleTimeType cycleTime)
		: cycleIndex(cycleIndex),
		  cycleTime(cycleTime)
	{}
};
