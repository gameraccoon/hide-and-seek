#pragma once

#include <string>

class World;

class LevelLoader
{
public:
	static void SaveWorld(const World& world, const std::string& levelName, const class ComponentFactory& componentFactory);
	static void LoadWorld(World& world, const std::string& levelName, const class ComponentFactory& componentFactory);
};
