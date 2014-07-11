#include "SqliteConnection.h"

#include <Debug/Log.h>

#include <sys/stat.h>

#include "SqliteDataReader.h"


SqliteConnection::SqliteConnection(std::string databaseFile)
{
	this->error = false;

	struct stat buffer;   
	if (stat(databaseFile.c_str(), &buffer) != 0)
	{
		this->error = true;
		Log::Instance().writeError(std::string("Database file isn't exist: ").append(databaseFile));
		throw new DatabaseErrorException();
	}

	int rc = sqlite3_open(databaseFile.c_str(), &database);
	
	if (rc != 0)
	{
		this->error = true;
		Log::Instance().writeError(std::string("Can't open database: ").append(sqlite3_errmsg(database)));
		throw new DatabaseErrorException();
	}
}


SqliteConnection::~SqliteConnection(void)
{
	sqlite3_close(database);
}

std::shared_ptr<SqlDataReader> SqliteConnection::execSql(std::string query)
{
	return std::shared_ptr<SqlDataReader>(new SqliteDataReader(query, database));
}
