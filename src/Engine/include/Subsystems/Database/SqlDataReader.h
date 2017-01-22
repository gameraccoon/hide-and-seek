#pragma once

#include <string>

#include <memory>

/**
* Some value returned by SqlDataReader.
* We can try to get this value as one of types.
*/
class DbValue
{
public:
	typedef std::shared_ptr<DbValue> Ptr;
	typedef std::weak_ptr<DbValue> WeakPtr;
public:
	virtual ~DbValue() {}
	virtual bool asBool() = 0;
	virtual int asInt() = 0;
	virtual float asFloat() = 0;
	virtual std::string asString() = 0;
	virtual const void* asVariant() = 0;
	virtual bool isNull() = 0;
};

/**
* Class that contains table returned by some sql query.
* After creation cursor points to the line before the first.
*/
class SqlDataReader
{
public:
	typedef std::shared_ptr<SqlDataReader> Ptr;

	virtual ~SqlDataReader() {}

	/**
	* Go to the next line
	*
	* @returns false if next line not exist
	*/
	virtual bool next() = 0;

	/**
	* Try to get value from column of curent line by the name
	*
	* @paramcolumnName name of table column
	*/
	virtual DbValue::Ptr getValueByName(const std::string& columnName) = 0;

	/**
	* Try to get value from column of curent line by the number
	*
	* @paramcolumnIndex index of table column (starting with 0)
	*/
	virtual DbValue::Ptr getValueByIndex(int columnIndex) = 0;
};
