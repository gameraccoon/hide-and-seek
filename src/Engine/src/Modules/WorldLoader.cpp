#include "Modules/WorldLoader.h"

#include <fstream>
#include <map>
#include <iomanip>

#include <filesystem>
#include <nlohmann/json.hpp>

#include <Core/World.h>

namespace WorldLoader
{
	static const std::filesystem::path MAPS_PATH = "./resources/maps";

	void SaveWorld(const World& world, const std::string& levelName, const ComponentFactory& componentFactory)
	{
		std::filesystem::path levelPath = std::filesystem::path(levelName);

		// if it's name, we search the map in maps folder
		if (levelName.find_first_of("/\\.") == std::string::npos)
		{
			levelPath = MAPS_PATH / (levelName + ".json");
		}

		try {
			std::ofstream mapFile(levelPath);
			nlohmann::json mapJson({{"world", world.toJson(componentFactory)}});

			mapFile << std::setw(4) << mapJson << std::endl;
		} catch (const std::exception& e) {
			LogError("Can't save level to file '%s': %s", levelPath.c_str(), e.what());
		}
	}

	void LoadWorld(World& world, const std::string& levelName, const ComponentFactory& componentFactory)
	{
		std::filesystem::path levelPath = std::filesystem::path(levelName);

		// if it's name, we search the map in maps folder
		if (levelName.find_first_of("/\\.") == std::string::npos)
		{
			levelPath = MAPS_PATH / (levelName + ".json");
		}

		try
		{
			std::ifstream mapFile(levelPath);
			nlohmann::json mapJson;
			mapFile >> mapJson;

			if (const auto& worldObject = mapJson.at("world"); worldObject.is_object())
			{
				world.fromJson(worldObject, componentFactory);
			}
		}
		catch(const nlohmann::detail::exception& e)
		{
			LogError("Can't parse '%s': %s", levelPath.c_str(), e.what());
		}
		catch(const std::exception& e)
		{
			LogError("Can't open '%s': %s", levelPath.c_str(), e.what());
		}
	}
}
