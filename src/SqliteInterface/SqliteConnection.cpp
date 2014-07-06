#include "SqliteConnection.h"

#include <Debug/Log.h>


SqliteConnection::SqliteConnection(std::string databaseFile)
{
	this->error = false;

	int rc = sqlite3_open(databaseFile.c_str(), &database);
	
	if (rc != 0)
	{
		this->error = true;
		Log::Instance().writeError(std::string("Can't open database: ").append(sqlite3_errmsg(database)));
	}
}


SqliteConnection::~SqliteConnection(void)
{
	sqlite3_close(database);
}

std::string SqliteConnection::getOneValue(std::string statement)
{
	if (error)
		throw new DatabaseErrorException();

	sqlite3_stmt *ppStmt;
	int rc = sqlite3_prepare(database, statement.c_str(), statement.length(), &ppStmt, 0);

	if (rc != 0)
	{
		Log::Instance().writeError(std::string("Can't execute statement: ").append(statement));
	}

	std::string singleValue = "";

	// go to the first line
	if (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
		// get value from the first column
		singleValue = reinterpret_cast<const char*>(sqlite3_column_text(ppStmt, 0));
	}
	else
	{
		Log::Instance().writeError("Can't read line from sqlite");
	}
	
	sqlite3_finalize(ppStmt);

	return singleValue;
}

bool SqliteConnection::isError()
{
	return this->error;
}