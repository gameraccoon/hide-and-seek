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
	SqliteConnection conn(std::string(databaseFileName).append(".db"));
	std::string value = conn.getOneValue(std::string("Select * from ").append(actorName));
	return value;
}