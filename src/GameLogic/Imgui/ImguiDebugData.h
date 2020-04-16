#pragma once

#include <vector>

class WorldHolder;
class SystemFrameRecords;

struct ImguiDebugData
{
	WorldHolder& worldHolder;
	SystemFrameRecords& systemRecords;
	std::vector<std::string> systemNames;
};
