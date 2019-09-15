#pragma once

#include <string>

class World;
class GameData;
class ComponentFactory;

namespace GameDataLoader
{
	void SaveWorld(const World& world, const std::string& levelName, const ComponentFactory& componentFactory);
	void LoadWorld(World& world, const std::string& levelName, const ComponentFactory& componentFactory);

	void SaveGameData(const GameData& gameData, const std::string& gameDataName, const ComponentFactory& componentFactory);
	void LoadGameData(GameData& gameData, const std::string& gameDataName, const ComponentFactory& componentFactory);
}
