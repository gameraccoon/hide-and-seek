#pragma once

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
	SqliteConnection(const std::string& databaseFile);
	virtual ~SqliteConnection() override;

	virtual std::shared_ptr<SqlDataReader> execQuery(const std::string& query) override;
	virtual void execSql(const std::string& statement) override;

	bool IsTableExists(const std::string& table);

	std::string GetSqliteErrMessage();

private:
	sqlite3 *_database;
};
