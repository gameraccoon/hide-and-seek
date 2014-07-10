#ifndef SQLITE_DATA_READER_H
#define SQLITE_DATA_READER_H

#include <string>

#include <sqlite3.h>

#include <Engine/Subsystems/Database/SqlDataReader.h>
#include <Engine/Subsystems/Database/DbConnection.h>

class SqliteValue : public DbValue
{
public:
	SqliteValue(sqlite3_stmt* ppStmt, int columnIndex);
	virtual ~SqliteValue() override;
	virtual bool asBool() override;
	virtual int asInt() override;
	virtual float asFloat() override;
	virtual std::string asString() override;
	virtual void* asVariant() override;
private:
	sqlite3_stmt* ppStmt;
	int columnIndex;
};

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class SqliteDataReader : public SqlDataReader
{
public:
	SqliteDataReader(std::string query, sqlite3* db);
	virtual ~SqliteDataReader();
	/**
	 * @throws TextureNotFoundException
	 */
	virtual bool next() override;

	virtual std::shared_ptr<DbValue> getValueByName(std::string columnName) override;
	
	virtual std::shared_ptr<DbValue> getValueByIndex(int columnIndex) override;

private:
	sqlite3_stmt *ppStmt;
};

#endif