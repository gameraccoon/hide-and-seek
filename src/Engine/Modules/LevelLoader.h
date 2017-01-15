#pragma once

#include "../Core/World.h"

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	static void save(World* world, const std::string levelName);
	static void load(World* world, const std::string levelName);
};
