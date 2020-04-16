#pragma once

#include <string>

#include "ECS/SystemsManager.h"

class SystemFrameRecords
{
public:
	void setRecordsLimit(unsigned int newLimit);

	void addFrame(SystemsFrameTime&& frameTime);
	std::vector<SystemsFrameTime>& getFramesRef();

	void printToFile(const std::vector<std::string>& systemNames, const std::string& fileName) const;
	void print(const std::vector<std::string>& systemNames, std::ostream& stream) const;

private:
	std::vector<SystemsFrameTime> mSystemFrameRecords;
	unsigned int mRecordsLimit = 0;
};
