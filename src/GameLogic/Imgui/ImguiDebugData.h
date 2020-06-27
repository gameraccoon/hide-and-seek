#pragma once

#include <vector>

class WorldHolder;
class SystemFrameRecords;
class TimeData;
class ComponentFactory;

struct ImguiDebugData
{
	WorldHolder& worldHolder;
	const TimeData& time;
	SystemFrameRecords& systemRecords;
	const ComponentFactory& componentFactory;
	std::vector<std::string> systemNames;
};
