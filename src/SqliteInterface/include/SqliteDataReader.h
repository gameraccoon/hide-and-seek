#pragma once

#include <string>

#include <Subsystems/Database/SqlDataReader.h>
#include <Subsystems/Database/DbConnection.h>

struct sqlite3_stmt;
struct sqlite3;

class SqliteValue : public DbValue
{
public:
	SqliteValue(sqlite3_stmt* ppStmt, int columnIndex);
	virtual ~SqliteValue() override;
	virtual bool asBool() override;
	virtual int asInt() override;
	virtual float asFloat() override;
	virtual std::string asString() override;
	virtual const void* asVariant() override;
	virtual bool isNull() override;
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
	SqliteDataReader(const std::string& query, sqlite3* db);
    virtual ~SqliteDataReader() override;
	/**
	* @throws TextureNotFoundException
	*/
	virtual bool next() override;

	virtual DbValue::Ptr getValueByName(const std::string& columnName) override;

	virtual DbValue::Ptr getValueByIndex(int columnIndex) override;

private:
	sqlite3_stmt *ppStmt;
};
