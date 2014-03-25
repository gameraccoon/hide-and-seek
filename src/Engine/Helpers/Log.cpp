#include "Log.h"

#include <fstream>
#include <ctime>

void Log::WriteError(std::string text)
{
	Log::WriteLine(std::string(" Error: ").append(text));
}

void Log::WriteWarning(std::string text)
{
	Log::WriteLine(std::string(" Warning: ").append(text));
}

void Log::WriteLog(std::string text)
{
	Log::WriteLine(std::string(" Log: ").append(text));
}

void Log::WriteLine(std::string text)
{
	std::ofstream logFile;
	logFile.open(std::string("./logs/").append("log.txt"), std::ios_base::app);

	logFile << std::time(0);
	logFile << text << std::endl;

	logFile.close();
}