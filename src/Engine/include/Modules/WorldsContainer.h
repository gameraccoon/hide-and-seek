#pragma once

#include <map>
#include "../Core/World.h"

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

	/*
	 * Turn off unusable operations
	 */
	WorldsContainer() = default;
	~WorldsContainer();
	WorldsContainer(const WorldsContainer&) = delete;
	WorldsContainer& operator=(const WorldsContainer&) = delete;
};
