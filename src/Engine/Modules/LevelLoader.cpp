#include "LevelLoader.h"

//#include <iostream>
#include <fstream>
//#include <iomanip>

#include "../Modules/ActorFactory.h"

LevelLoader::LevelLoader(void)
{
}


LevelLoader::~LevelLoader(void)
{
}

void LevelLoader::save(World* world, const std::string levelName)
{
	// saving all actors in this world
	std::ofstream mapFile;
	mapFile.open(std::string("./maps/").append(levelName).append(std::string(".map")));
	for (std::set<IActor*>::iterator i = world->allActors.begin(), iEnd = world->allActors.end(); i != iEnd; i++)
	{
		mapFile << (*i)->getClassID();
		mapFile << " ";
		mapFile << (*i)->getLocation().x;
		mapFile << " ";
		mapFile << (*i)->getLocation().y;
		mapFile << " ";
		mapFile << (*i)->getScale().x;
		mapFile << " ";
		mapFile << (*i)->getScale().y;
		mapFile << " ";
		mapFile << (*i)->getRotation().getValue();
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
	for (std::set<PathPoint*>::iterator j = world->navigationMap.begin(), jEnd = world->navigationMap.end(); j != jEnd; j++)
	{
		points.insert(PathPointMap::value_type((*j), i));
		pathsFile << i;
		pathsFile << " ";
		pathsFile << (*j)->location.x;
		pathsFile << " ";
		pathsFile << (*j)->location.y;
		pathsFile << "\n";
		i++;
	}

	pathsFile << "-1\n";

	// save connections of pathpoints
	for (std::set<PathPoint*>::iterator j = world->navigationMap.begin(), jEnd = world->navigationMap.end(); j != jEnd; i++)
	{
		int firstIndex = points.at(*j);

		for (std::set<PathPoint*>::iterator k = (*j)->legalPoints.begin(), kEnd = (*j)->legalPoints.end(); k != kEnd; k++)
		{
			int secondIndex = points.at(*k);

			pathsFile << firstIndex;
			pathsFile << " ";
			pathsFile << secondIndex;
			pathsFile << "\n";
		}
	}

	pathsFile.close();
}

void LevelLoader::load(World* world, const std::string levelName)
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
		std::string className;
		
		float xPos, yPos;
		float xScale, yScale;
		float angle;
		mapFile >> className;
		mapFile >> xPos;
		mapFile >> yPos;
		mapFile >> xScale;
		mapFile >> yScale;
		mapFile >> angle;

		try
		{
			ActorFactory::createActor(className, world, Vector2D(xPos, yPos), Vector2D(xScale, yScale), Rotator(angle));
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
		points.at(point1)->connect(points.at(point2));
	}

	// put paths into world
	for (int i = 0, pointsSize = points.size(); i < pointsSize; i++)
	{
		world->addPathPoint(points.at(i));
	}

	pathsFile.close();
}