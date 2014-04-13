#include "WorldsContainer.h"

WorldsContainer* WorldsContainer::singleInstance = NULL;

WorldsContainer::WorldsContainer() {}
WorldsContainer::WorldsContainer(const WorldsContainer&) {}
WorldsContainer::~WorldsContainer() {}
WorldsContainer& WorldsContainer::operator=(const WorldsContainer&)
{
	return *WorldsContainer::singleInstance;
}

WorldsContainer& WorldsContainer::Container()
{
	if (WorldsContainer::singleInstance == NULL)
	{
		WorldsContainer::singleInstance = new WorldsContainer();
	}

	return *WorldsContainer::singleInstance;
}


void WorldsContainer::insertWorld(World* world, std::string name)
{
	this->worlds.insert(WorldsMap::value_type(name, world)).second;
}

World* WorldsContainer::getWorldByName(std::string name)
{
	WorldsMap::const_iterator it = this->worlds.find(name);
	
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
