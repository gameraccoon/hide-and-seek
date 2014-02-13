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

void LevelLoader::Save(World* world, const std::string levelName)
{
	// saving all actors in this world
	std::ofstream mapFile;
	mapFile.open(std::string("./maps/").append(levelName).append(std::string(".map")));
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
	typedef std::map<PathPoint*, int> PathPointMap;
	PathPointMap points;
	
	std::ofstream pathsFile;
	pathsFile.open(std::string("./maps/").append(levelName).append(std::string(".paths")));

	int i = 0;
	// save locations of pathpoints
	for (std::set<PathPoint*>::iterator it = world->NavigationMap.begin(), end = world->NavigationMap.end(); it != end; it++)
	{
		points.insert(PathPointMap::value_type((*it), i));
		pathsFile << i;
		pathsFile << " ";
		pathsFile << (*it)->Location.X;
		pathsFile << " ";
		pathsFile << (*it)->Location.Y;
		pathsFile << "\n";
		i++;
	}

	pathsFile << "-1\n";

	// save connections of pathpoints
	for (std::set<PathPoint*>::iterator it = world->NavigationMap.begin(), end = world->NavigationMap.end(); it != end; it++)
	{
		int firstIndex = points.at(*it);
		for (std::set<PathPoint*>::iterator it2 = (*it)->LegalPoints.begin(), end2 = (*it)->LegalPoints.end(); it2 != end2; it2++)
		{
			int secondIndex = points.at(*it2);

			pathsFile << firstIndex;
			pathsFile << " ";
			pathsFile << secondIndex;
			pathsFile << "\n";
		}
		
		i++;
	}

	pathsFile.close();
}

void LevelLoader::Load(World* world, const std::string levelName)
{
	// loading actors
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

	// loading paths
	typedef std::map<int, PathPoint*> PathPointMap;
	PathPointMap points;

	std::ifstream pathsFile;
	pathsFile.open(std::string("./maps/").append(levelName).append(std::string(".paths")));
	while (!pathsFile.eof())
	{
		int point;
		float xPos, yPos;
		pathsFile >> point;

		// leave cycle when reached divider
		if (point == -1) break;

		pathsFile >> xPos;
		pathsFile >> yPos;

		points.insert(PathPointMap::value_type(point, new PathPoint(Vector2D(xPos, yPos))));
	}

	// load connections and link pathpoints
	while (!pathsFile.eof())
	{
		int point1, point2;
		pathsFile >> point1;
		pathsFile >> point2;

		// create way from point1 to point2
		points.at(point1)->Connect(points.at(point2));
	}

	// put paths into world
	for (int i = 0, pointsSize = points.size(); i < pointsSize; i++)
	{
		world->AddPathPoint(points.at(i));
	}

	pathsFile.close();
}