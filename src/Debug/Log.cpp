#include "Log.h"

#include <fstream>
#include <ctime>

Log* Log::singleInstance = nullptr;
bool Log::isDestroyed = false;
bool Log::isFirstLife = true;

Log::Log()
{
	const std::string LOG_FILE = std::string("./logs/").append("log.txt"); 

	if (this->isFirstLife)
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::trunc);
		this->writeInit("Log file created");
	}
	else
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::app);
	}
}

Log::~Log()
{
	this->logFileStream->close();
	delete this->logFileStream;

	Log::singleInstance = nullptr;
	Log::isDestroyed = true;
	Log::isFirstLife = false;
}

Log& Log::Instance()
{
	// if we haven't instance of the Log
	if (Log::singleInstance == nullptr)
	{
		if (isDestroyed)
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

	return *Log::singleInstance;
}

void Log::create()
{
	/*
	 * Real object that be initialized on first usage
	 * and it be automatically destroyed on program shutdown
	 */
	static Log theInstance;
	Log::singleInstance = &theInstance;
}

void Log::onDeadReference()
{
	// Get the old singletone location in the memory (ash of the phoenix)
	Log::create();
	// Create new singletone on this place
	new (Log::singleInstance) Log;
	// Say that we want to destroy this singletone on the application shutdown
	atexit(killPhoenixSingletone);
	// Say that the singletone is ready to use
	Log::isDestroyed = false;
}

void Log::killPhoenixSingletone()
{
	Log::singleInstance->~Log();
	operator delete(Log::singleInstance);
}

void Log::writeError(const std::string& text)
{
	this->writeLine(std::string(" Error: ").append(text));
}

void Log::writeWarning(const std::string& text)
{
	this->writeLine(std::string(" Warning: ").append(text));
}

void Log::writeLog(const std::string& text)
{
	this->writeLine(std::string(" Log: ").append(text));
}

void Log::writeInit(const std::string& text)
{
	this->writeLine(std::string(" Init: ").append(text));
}

void Log::writeLine(const std::string& text)
{
	if (this->logFileStream->is_open())
	{
		char dateStr[9];
		char timeStr[9];
		_strdate_s(dateStr);
		*this->logFileStream << dateStr << " ";
		_strtime_s(timeStr);
		*this->logFileStream << timeStr;
		*this->logFileStream << text << std::endl;
	}
}
