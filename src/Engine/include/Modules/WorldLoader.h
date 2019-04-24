#pragma once

#include <string>

class World;
class ComponentFactory;

namespace WorldLoader
{
	void SaveWorld(const World& world, const std::string& levelName, const ComponentFactory& componentFactory);
	void LoadWorld(World& world, const std::string& levelName, const ComponentFactory& componentFactory);
}
