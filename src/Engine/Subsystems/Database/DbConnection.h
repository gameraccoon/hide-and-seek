#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <string>

#include <memory>

#include "SqlDataReader.h"

class DatabaseErrorException : public std::exception { };

/**
 *
 */
class DbConnection
{
public:
	virtual ~DbConnection() {}

	virtual std::shared_ptr<SqlDataReader> execSql(std::string query) = 0;
};

#endif