#ifndef CHRONICLER_H
#define CHRONICLER_H

#include <string>

class Log
{
public:
	static void WriteError(std::string);
	static void WriteWarning(std::string);
	static void WriteLog(std::string);

private:
	static void WriteLine(std::string text);
};

#endif