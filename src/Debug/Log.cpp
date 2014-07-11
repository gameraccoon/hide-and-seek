#include "Log.h"

#include <fstream>
#include <ctime>

Log* Log::singleInstance = nullptr;
bool Log::isDestroyed = false;

Log::Log()
{
	static const std::string LOG_FILE = std::string("./logs/").append("log.txt"); 

	this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::trunc);
	this->writeInit("Log file created");
}

Log::~Log()
{
	this->logFileStream->close();
	delete this->logFileStream;

	singleInstance = nullptr;
	isDestroyed = true;
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
			Log::singleInstance = new Log();
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
	new (singleInstance) Log;
	// Say that we want to destroy this singletone on the application shutdown
	atexit(killPhoenixSingletone);
	// Say that the singletone is ready to use
	Log::isDestroyed = false;
}

void Log::killPhoenixSingletone()
{
	delete Log::singleInstance;
}

void Log::writeError(std::string text)
{
	this->writeLine(std::string(" Error: ").append(text));
}

void Log::writeWarning(std::string text)
{
	this->writeLine(std::string(" Warning: ").append(text));
}

void Log::writeLog(std::string text)
{
	this->writeLine(std::string(" Log: ").append(text));
}

void Log::writeInit(std::string text)
{
	this->writeLine(std::string(" Init: ").append(text));
}

void Log::writeLine(std::string text)
{
	if (this->logFileStream->is_open())
	{
		*this->logFileStream << std::time(0);
		*this->logFileStream << text << std::endl;
	}
}
