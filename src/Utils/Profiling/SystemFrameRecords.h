#pragma once

#include <string>

#include "ECS/SystemsManager.h"

class SystemFrameRecords
{
public:
	void addFrame(SystemsFrameTime&& frameTime)
	{
		mSystemFrameRecords.emplace_back(std::forward<SystemsFrameTime>(frameTime));
	}

	void printToFile(const std::vector<std::string>& systemNames, const std::string& fileName);
	void print(const std::vector<std::string>& systemNames, std::ostream& stream);

private:
	std::vector<SystemsFrameTime> mSystemFrameRecords;
};
