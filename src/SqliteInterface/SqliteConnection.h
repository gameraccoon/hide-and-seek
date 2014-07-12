#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

struct sqlite3;

#include <Engine/Subsystems/Database/DbConnection.h>

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class SqliteConnection : public DbConnection
{
public:
	SqliteConnection(std::string databaseFile);
	virtual ~SqliteConnection(void) override;

	virtual std::shared_ptr<SqlDataReader> execSql(std::string query) override;
private:
	sqlite3 *database;
};

#endif