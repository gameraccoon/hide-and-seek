#include "Utils/World/WorldLoader.h"

#include <fstream>
#include <map>
#include <iomanip>

#include <experimental/filesystem>
#include <nlohmann/json.hpp>

#include "Debug/Log.h"
#include "GameData/World.h"

namespace WorldLoader
{
	static const std::experimental::filesystem::path MAPS_PATH = "./resources/maps";

	void SaveWorld(const World& world, const std::string& levelName, const ComponentFactory& componentFactory)
	{
		namespace fs = std::experimental::filesystem;
		fs::path levelPath(levelName);

		// if it's name, we save to maps folder
		if (levelName.find_first_of("/\\.") == std::string::npos)
		{
			levelPath = MAPS_PATH / (levelName + ".json");
		}
		else
		{
			if (!levelPath.has_extension())
			{
				levelPath = levelPath.string() + ".json";
			}
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
		namespace fs = std::experimental::filesystem;
		fs::path levelPath(levelName);

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
