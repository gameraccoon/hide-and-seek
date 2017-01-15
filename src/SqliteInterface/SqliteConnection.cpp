#include "SqliteConnection.h"

#include <Debug/Log.h>

#include <sys/stat.h>
#include <sqlite/sqlite3.h>

#include "SqliteDataReader.h"


SqliteConnection::SqliteConnection(const std::string& databaseFile)
{
	struct stat buffer;
	if (stat(databaseFile.c_str(), &buffer) != 0)
	{
		Log::Instance().writeLog(std::string("Database file isn't exist (creating): ").append(databaseFile));
	}

	int rc = sqlite3_open(databaseFile.c_str(), &_database);

	if (rc != 0)
	{
		Log::Instance().writeError(std::string("Can't open database: ").append(sqlite3_errmsg(_database)));
	}
}

SqliteConnection::~SqliteConnection()
{
	sqlite3_close(_database);
}

std::shared_ptr<SqlDataReader> SqliteConnection::execQuery(const std::string& query)
{
	return std::shared_ptr<SqlDataReader>(new SqliteDataReader(query, _database));
}

void SqliteConnection::execSql(const std::string& statement)
{
	if (sqlite3_exec(_database, statement.c_str(), nullptr, nullptr, nullptr) != 0)
	{
		Log::Instance().writeError(std::string("Unable to execute SQL statement \"").append(statement)
			.append("\" ").append(sqlite3_errmsg(_database)));
	}
}

bool SqliteConnection::IsTableExists(const std::string& table)
{
	SqliteDataReader dbReader(std::string(
		"select * from sqlite_master where type = 'table' and tbl_name = '").append(table).append("'"), _database);
	return (dbReader.next());
}

std::string SqliteConnection::GetSqliteErrMessage()
{
	return std::string(sqlite3_errmsg(_database));
}
