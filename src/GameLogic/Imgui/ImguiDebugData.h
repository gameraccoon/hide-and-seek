#pragma once

#include <vector>

class WorldHolder;
class SystemFrameRecords;
class TimeData;

struct ImguiDebugData
{
	WorldHolder& worldHolder;
	const TimeData& time;
	SystemFrameRecords& systemRecords;
	std::vector<std::string> systemNames;
};
