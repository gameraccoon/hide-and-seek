#include "SqliteDataReader.h"

#include <Debug/Log.h>

SqliteValue::SqliteValue(sqlite3_stmt* ppStmt, int columnIndex)
{
	this->ppStmt = ppStmt;
	this->columnIndex = columnIndex;
}

SqliteValue::~SqliteValue()
{
}

bool SqliteValue::asBool()
{
	return false;
}

int SqliteValue::asInt()
{
	return 0;
}

float SqliteValue::asFloat()
{
	return 0.0f;
}

std::string SqliteValue::asString()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(this->ppStmt, this->columnIndex));
}

void* SqliteValue::asVariant()
{
	return nullptr;
}


SqliteDataReader::SqliteDataReader(std::string query, sqlite3* db)
{
	int rc = sqlite3_prepare(db, query.c_str(), query.length(), &this->ppStmt, 0);

	if (rc != 0)
	{
		Log::Instance().writeError(std::string("Can't execute query: ").append(query));
	}
}

SqliteDataReader::~SqliteDataReader()
{
	sqlite3_finalize(this->ppStmt);
}

bool SqliteDataReader::next()
{
	// go to the next line
	bool result = (sqlite3_step(this->ppStmt) == SQLITE_ROW);
	
	return result;
}

std::shared_ptr<DbValue> SqliteDataReader::getValueByName(std::string columnName)
{	
	return nullptr;
}

std::shared_ptr<DbValue> SqliteDataReader::getValueByIndex(int columnIndex)
{	
	return std::shared_ptr<DbValue>(new SqliteValue(this->ppStmt, columnIndex));
}
/*
std::string SqliteDataReader::getStateName(std::string actorName)
{
	SqliteConnection conn(std::string(databaseFileName).append(".db"));
	std::string value = conn.getOneValue(std::string("Select * from ").append(actorName));
	return value;
}*/