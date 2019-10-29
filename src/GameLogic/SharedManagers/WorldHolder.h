#pragma once

#include "Base/Debug/Assert.h"

class World;
class GameData;

class WorldHolder
{
public:
	WorldHolder(World* world, GameData& gameData)
		: world(world)
		, gameData(gameData)
	{
		AssertFatal(world, "World is not set");
	}

	void setWorld(World* newWorld)
	{
		AssertFatal(newWorld, "World is not set");
		world = newWorld;
	}

	World& getWorld()
	{
		return *world;
	}

	GameData& getGameData()
	{
		return gameData;
	}

private:
	World* world;
	GameData& gameData;
};
