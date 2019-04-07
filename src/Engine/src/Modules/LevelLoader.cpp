#include "Modules/LevelLoader.h"

#include <fstream>
#include <map>
#include <iomanip>

#include <filesystem>
#include <nlohmann/json.hpp>

#include <Core/World.h>

static const std::filesystem::path MAPS_PATH = "./resources/maps";

void LevelLoader::SaveWorld(World* world, const std::string& levelName)
{
	std::filesystem::path levelPath = MAPS_PATH / (levelName + ".json");
	try {
		std::ofstream mapFile(levelPath);
		nlohmann::json mapJson({{"world", *world}});

		mapFile << std::setw(4) << mapJson << std::endl;
	} catch (const std::exception& e) {
		LogError("Can't save level to file '%s': %s", levelPath.c_str(), e.what());
	}
}

void LevelLoader::LoadWorld(World* world, const std::string& levelName)
{
	std::filesystem::path levelPath = MAPS_PATH / (levelName + ".json");

	try
	{
		std::ifstream mapFile(levelPath);
		nlohmann::json mapJson;
		mapFile >> mapJson;

		auto entities = mapJson.at("world").get_to(*world);
	}
	catch(const nlohmann::detail::parse_error& e)
	{
		LogError("Can't parse '%s': %s", levelPath.c_str(), e.what());
	}
	catch(const std::exception& e)
	{
		LogError("Can't open file '%s': %s", levelPath.c_str(), e.what());
	}
}
