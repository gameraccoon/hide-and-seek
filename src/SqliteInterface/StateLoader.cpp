#include "StateLoader.h"

#include "SqliteConnection.h"


StateLoader::StateLoader(std::string databaseFileName)
{
	this->databaseFileName = databaseFileName;
}


StateLoader::~StateLoader(void)
{
}

std::string StateLoader::getStateName(std::string actorName)
{
	SqliteConnection conn("testdb.db");
	std::string value = conn.getOneValue("Select * from test");
	return value;
}