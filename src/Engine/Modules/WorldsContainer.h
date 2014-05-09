#ifndef WORLDS_CONTAINER_H
#define WORLDS_CONTAINER_H

#include <map>
#include "..\Core\World.h"

/**
 * Container for created worlds.
 * Is singletone.
 */
class WorldsContainer
{
public:
	static WorldsContainer& Container();

	void insertWorld(World* world, std::string name);
	World* getWorldByName(std::string name);
	void removeWorld(std::string name);
private:

	typedef std::map<std::string, World*> WorldsMap;
	std::map<std::string, World*> worlds;

	static WorldsContainer* singleInstance;

	/*
	 * Turn off unusable operations
	 */
	WorldsContainer();
	WorldsContainer(const WorldsContainer&);
	~WorldsContainer();
	WorldsContainer& operator=(const WorldsContainer&);
};

#endif