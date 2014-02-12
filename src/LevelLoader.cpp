#include "LevelLoader.h"

//#include <iostream>
#include <fstream>
//#include <iomanip>

#include "../src/ActorFactory.h"

LevelLoader::LevelLoader(void)
{
}


LevelLoader::~LevelLoader(void)
{
}

void LevelLoader::Save(World* world, const std::string name)
{
	// saving all actors in this world
	std::ofstream mapFile;
	mapFile.open(std::string("./maps/").append(name).append(std::string(".map")));
	for (std::set<IActor*>::iterator it = world->AllActors.begin(), end = world->AllActors.end(); it != end; it++)
	{
		mapFile << (*it)->GetClassID();
		mapFile << " ";
		mapFile << (*it)->GetLocation().X;
		mapFile << " ";
		mapFile << (*it)->GetLocation().Y;
		mapFile << " ";
		mapFile << (*it)->GetScale().X;
		mapFile << " ";
		mapFile << (*it)->GetScale().Y;
		mapFile << " ";
		mapFile << (*it)->GetRotation().GetValue();
		mapFile << "\n";
	}
	mapFile.close();

	// saving paths
	std::ofstream pathsFile;
	pathsFile.open(std::string("./maps/").append(name).append(std::string(".paths")));
	for (std::set<PathPoint*>::iterator it = world->NavigationMap.begin(), end = world->NavigationMap.end(); it != end; it++)
	{
		pathsFile << (*it)->Location.X;
		pathsFile << " ";
		pathsFile << (*it)->Location.Y;
		pathsFile << "\n";
	}
	pathsFile.close();
}

void LevelLoader::Load(World* world, const std::string levelName)
{
// saving all actors in this world
	std::ifstream mapFile;
	try
	{
		mapFile.open(std::string("./maps/").append(levelName).append(std::string(".map")));
	}
	catch(std::exception)
	{
		return;
	}

	while (!mapFile.eof())
	{
		std::string ClassName;
		
		float xPos, yPos;
		float xScale, yScale;
		float angle;
		mapFile >> ClassName;
		mapFile >> xPos;
		mapFile >> yPos;
		mapFile >> xScale;
		mapFile >> yScale;
		mapFile >> angle;

		try
		{
			ActorFactory::CreateActor(ClassName, world, Vector2D(xPos, yPos), Vector2D(xScale, yScale), Rotator(angle));
		}
		catch (std::runtime_error)
		{
			// skip unknown classes
		}
		
	}
	mapFile.close();

	// saving paths
	/*std::ofstream pathsFile;
	pathsFile.open(std::string("./maps/").append(name).append(std::string(".paths")));
	for (std::set<PathPoint*>::iterator it = NavigationMap.begin(), end = NavigationMap.end(); it != end; it++)
	{
		pathsFile << (*it)->Location.X;
		pathsFile << " ";
		pathsFile << (*it)->Location.Y;
		pathsFile << "\n";
	}
	pathsFile.close();*/
}