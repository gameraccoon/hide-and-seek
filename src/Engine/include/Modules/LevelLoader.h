#pragma once

#include <string>

class World;

class LevelLoader
{
public:
	static void SaveWorld(World* world, const std::string& levelName);
	static void LoadWorld(World* world, const std::string& levelName);
};
