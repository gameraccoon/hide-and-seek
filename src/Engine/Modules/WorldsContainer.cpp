#include "WorldsContainer.h"

#include <Debug/Log.h>

WorldsContainer::WorldsContainer() {}
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
	this->worlds.insert(WorldsMap::value_type(name, world)).second;
}

World* WorldsContainer::getWorldByName(std::string name)
{
	auto it = this->worlds.find(name);
	
	if (it == this->worlds.end())
	{
		throw std::runtime_error("Unknown actor identefier");
	}

	return (it->second);
}

void WorldsContainer::removeWorld(std::string name)
{
	this->worlds.erase(name);
}
