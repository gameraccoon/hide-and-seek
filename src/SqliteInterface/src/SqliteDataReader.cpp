#include "SqliteDataReader.h"

#include <sqlite/sqlite3.h>

#include <Log.h>

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
	return (sqlite3_column_int(this->ppStmt, this->columnIndex) != 0);
}

int SqliteValue::asInt()
{
	return sqlite3_column_int(this->ppStmt, this->columnIndex);
}

float SqliteValue::asFloat()
{
	return (float)sqlite3_column_double(this->ppStmt, this->columnIndex);
}

std::string SqliteValue::asString()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(this->ppStmt, this->columnIndex));
}

const void* SqliteValue::asVariant()
{
	return sqlite3_column_blob(this->ppStmt, this->columnIndex);
}

SqliteDataReader::SqliteDataReader(const std::string& query, sqlite3* db)
{
	int rc = sqlite3_prepare(db, query.c_str(), (int)query.length(), &this->ppStmt, 0);

	if (rc != 0)
	{
		Log::Instance().writeError(std::string("Unable to execute SQL query \"").append(query)
			.append("\" ").append(sqlite3_errmsg(db)));
	}
}

bool SqliteValue::isNull()
{
	return sqlite3_column_type(ppStmt, columnIndex) == SQLITE_NULL;
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

DbValue::Ptr SqliteDataReader::getValueByName(const std::string& columnName)
{
	for (int columnIndex = 0, columnsCount = sqlite3_column_count(this->ppStmt);
		columnIndex < columnsCount;
		columnIndex++)
	{
		std::string sTemp = sqlite3_column_name(this->ppStmt, columnIndex);
		if (sTemp == columnName)
		{
			return DbValue::Ptr(new SqliteValue(this->ppStmt, columnIndex));
		}
	}

	Log::Instance().writeError("SqliteDataReader error: Column not found: " + columnName);
	return nullptr;
}

DbValue::Ptr SqliteDataReader::getValueByIndex(int columnIndex)
{
	if (columnIndex >= sqlite3_column_count(this->ppStmt))
	{
		Log::Instance().writeError("SqliteDataReader error: Column not found: " + std::to_string(columnIndex));
	}

	return DbValue::Ptr(new SqliteValue(this->ppStmt, columnIndex));
}
