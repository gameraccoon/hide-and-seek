#include "Modules/WorldsContainer.h"

#include <Log.h>

WorldsContainer::~WorldsContainer()
{
	Log::Instance().writeLog("WorldsContainer destroyed");
}

WorldsContainer& WorldsContainer::Container()
{
	static WorldsContainer singleInstance;
	return singleInstance;
}


void WorldsContainer::insertWorld(World* world, std::string name)
{
	worlds.insert(WorldsMap::value_type(name, world)).second;
}

World* WorldsContainer::getWorldByName(std::string name)
{
	auto it = worlds.find(name);
	
	if (it == worlds.end())
	{
		throw std::runtime_error("Unknown actor identefier");
	}

	return (it->second);
}

void WorldsContainer::removeWorld(std::string name)
{
	worlds.erase(name);
}
