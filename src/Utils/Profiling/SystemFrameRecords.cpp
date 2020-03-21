#include "Base/precomp.h"

#include "Utils/Profiling/SystemFrameRecords.h"

#include <fstream>

void SystemFrameRecords::printToFile(const std::vector<std::string>& systemNames, const std::string& fileName)
{
	std::ofstream outStream(fileName);
	print(std::move(systemNames), outStream);
}

void SystemFrameRecords::print(const std::vector<std::string>& systemNames, std::ostream& outStream)
{
	// printing in CSV format
	outStream << "\"Total\"";
	for (const auto& systemName : systemNames)
	{
		outStream << ",\"" << systemName << "\"";
	}
	outStream << "\n";

	for (const auto& frameRecord : mSystemFrameRecords)
	{
		outStream << frameRecord.frameTime.count();
		for (const auto& systemTime : frameRecord.systemTime)
		{
			outStream << "," << systemTime.count();
		}
		outStream << "\n";
	}
}
