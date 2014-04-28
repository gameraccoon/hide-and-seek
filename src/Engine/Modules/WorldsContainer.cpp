#include "WorldsContainer.h"

WorldsContainer* WorldsContainer::singleInstance = nullptr;

WorldsContainer::WorldsContainer() {}
WorldsContainer::WorldsContainer(const WorldsContainer&) {}
WorldsContainer::~WorldsContainer() {}
WorldsContainer& WorldsContainer::operator=(const WorldsContainer&)
{
	return *WorldsContainer::singleInstance;
}

WorldsContainer& WorldsContainer::Container()
{
	if (WorldsContainer::singleInstance == nullptr)
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
