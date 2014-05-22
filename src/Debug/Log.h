#ifndef LOG_H
#define LOG_H

#include <string>

class Log
{
public:
	static void WriteError(std::string text);
	static void WriteWarning(std::string text);
	static void WriteLog(std::string text);

private:
	static void WriteLine(std::string text);
};

#endif