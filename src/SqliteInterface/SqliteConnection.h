#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include <sqlite3.h>

#include <string>

class DatabaseErrorException : public std::exception { };
class SqlStatementException : public std::exception { };

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class SqliteConnection
{
public:
	SqliteConnection(std::string databaseFile);
	~SqliteConnection(void);

	bool isError();
	std::string getOneValue(std::string statement);
private:
	sqlite3 *database;
	bool error;
};

#endif