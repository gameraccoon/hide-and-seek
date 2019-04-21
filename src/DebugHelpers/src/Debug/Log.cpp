#include "Debug/Log.h"

#include <fstream>
#include <chrono>
#include <iomanip>
#include <filesystem>

Log* Log::mSingleInstance = nullptr;
bool Log::mIsDestroyed = false;
bool Log::mIsFirstLife = true;

Log::Log()
{
    const std::string LOG_FILE = std::string("./logs/").append("log.txt");

	if (mIsFirstLife)
	{
        namespace fs = std::filesystem;
        if (!fs::is_directory("./logs") || !fs::exists("./logs")) {
            fs::create_directory("logs");
        }

		mLogFileStream = new std::ofstream(LOG_FILE, std::ios_base::trunc);
		writeInit("Log file created");
	}
	else
	{
		mLogFileStream = new std::ofstream(LOG_FILE, std::ios_base::app);
	}
}

Log::~Log()
{
	mLogFileStream->close();
	delete mLogFileStream;

	Log::mSingleInstance = nullptr;
	Log::mIsDestroyed = true;
	Log::mIsFirstLife = false;
}

Log& Log::Instance()
{
    // if we don't have the instance of the Log
	if (Log::mSingleInstance == nullptr)
	{
		if (mIsDestroyed)
		{
			// the Log was destroyed
			Log::onDeadReference();
		}
		else
		{
			// first access, create single instance
			Log::create();
		}
	}

	return *Log::mSingleInstance;
}

void Log::create()
{
	/*
	 * Real object that be initialized on first usage
	 * and it be automatically destroyed on program shutdown
	 */
	static Log theInstance;
	Log::mSingleInstance = &theInstance;
}

void Log::onDeadReference()
{
	// Get the old singletone location in the memory (ash of the phoenix)
	Log::create();
	// Create new singletone on this place
	new (Log::mSingleInstance) Log;
	// Say that we want to destroy this singletone on the application shutdown
	atexit(killPhoenixSingletone);
	// Say that the singletone is ready to use
	Log::mIsDestroyed = false;
}

void Log::killPhoenixSingletone()
{
	Log::mSingleInstance->~Log();
	operator delete(Log::mSingleInstance);
}

void Log::writeError(const std::string& text)
{
	writeLine(std::string(" Error: ").append(text));
}

void Log::writeWarning(const std::string& text)
{
	writeLine(std::string(" Warning: ").append(text));
}

void Log::writeLog(const std::string& text)
{
	writeLine(std::string(" Log: ").append(text));
}

void Log::writeInit(const std::string& text)
{
	writeLine(std::string(" Init: ").append(text));
}

void Log::writeLine(const std::string& text)
{
	if (mLogFileStream->is_open())
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        *mLogFileStream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
		*mLogFileStream << text << std::endl;
	}

	std::clog << text << "\n";
}
