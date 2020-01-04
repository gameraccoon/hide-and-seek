#pragma once

#include <string>
#include "Base/Types/String/StringHelpers.h"

/**
 * Class that helps to write log messages.
 * Log is a Singletone "phoenix".
 * (see Alexandrescu "Modern C++ Design: Generic Programming and Design Patterns Applied")
 */
class Log
{
public:
	/**
	 * Return single instance of class Log
	 * Usage: Log::Instance().function(...);
	 */
	static Log& Instance();

	/* Logging functions */
	void writeError(const std::string& text);
	void writeWarning(const std::string& text);
	void writeLog(const std::string& text);
	void writeInit(const std::string& text);

private:
	/** Write line with timestamp */
	void writeLine(const std::string& text);

	/** Filestream that holds the logfile handler */
	std::ofstream *mLogFileStream;

	/** Construct singletone information */
	static void create();

	/** Function runs when user tries to get access to destroyed singletone */
	static void onDeadReference();

	/** function that destroy singletone with ability to construct it in future */
	static void killPhoenixSingletone();

	/** Single instance of class log */
	static Log* mSingleInstance;

	/** Flag shows that singletone was created and destroyed */
	static bool mIsDestroyed;
	/** Flag shows that this is the first life of this singletone */
	static bool mIsFirstLife;

	/* Turn off unusable operations */
	Log();
	~Log();
	Log(const Log&) = delete;
	void operator=(const Log&) = delete;
};

template<typename... Args>
void LogInit(const std::string& message, Args... args)
{
	Log::Instance().writeInit(FormatString(message, std::forward<Args>(args)...));
}

template<typename... Args>
void LogInfo(const std::string& message, Args... args)
{
	Log::Instance().writeLog(FormatString(message, std::forward<Args>(args)...));
}

template<typename... Args>
void LogWarning(const std::string& message, Args... args)
{
	Log::Instance().writeWarning(FormatString(message, std::forward<Args>(args)...));
}

template<typename... Args>
void LogError(const std::string& message, Args... args)
{
	Log::Instance().writeError(FormatString(message, std::forward<Args>(args)...));
}

template<typename... Args>
void LogAssertHelper(const char* condition, const char* file, size_t line, const std::string& message, Args... args)
{
	Log::Instance().writeError(FormatString(std::string("Assertion failed '%s' %s:%d. Message: '").append(message), condition, file, line, std::forward<Args>(args)...));
}
