#pragma once

#include <string>

class World;
class GameData;
struct ComponentSerializersHolder;

namespace GameDataLoader
{
	void SaveWorld(const World& world, const std::string& levelName, const ComponentSerializersHolder& componentSerializers);
	void LoadWorld(World& world, const std::string& levelName, const ComponentSerializersHolder& componentSerializers);

	void SaveGameData(const GameData& gameData, const std::string& gameDataName, const ComponentSerializersHolder& componentSerializers);
	void LoadGameData(GameData& gameData, const std::string& gameDataName, const ComponentSerializersHolder& componentSerializers);
}
