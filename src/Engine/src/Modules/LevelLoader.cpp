#include "Modules/LevelLoader.h"

//#include <iostream>
#include <fstream>
//#include <iomanip>

#include "Modules/ActorFactory.h"

void LevelLoader::save(World* world, const std::string levelName)
{
	// saving all actors in this world
	std::ofstream mapFile;
	mapFile.open(std::string("./maps/").append(levelName).append(std::string(".map")));
	for (const auto& actor : world->getAllActors())
	{
		mapFile << actor->getClassID();
		mapFile << " ";
		mapFile << actor->getLocation().x;
		mapFile << " ";
		mapFile << actor->getLocation().y;
		mapFile << " ";
		mapFile << actor->getScale().x;
		mapFile << " ";
		mapFile << actor->getScale().y;
		mapFile << " ";
		mapFile << actor->getRotation().getValue();
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
	for (const auto &navPoint : world->getNavigationMap())
	{
		points.insert(PathPointMap::value_type(navPoint.get(), i));

		pathsFile << i;
		pathsFile << " ";
		pathsFile << navPoint->location.x;
		pathsFile << " ";
		pathsFile << navPoint->location.y;
		pathsFile << "\n";
		i++;
	}

	pathsFile << "-1\n";

	// save connections of pathpoints
	for (auto const &point : points)
	{
		int firstIndex = points.at(point.first);

		for (auto const &connection : point.first->legalPoints)
		{
			int secondIndex = points.at(connection);

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
	catch(std::exception*)
	{
		return;
	}

	while (!mapFile.eof())
	{
		std::string className;

		mapFile >> className;

		if (className == "")
			continue;
		
		float xPos, yPos;
		float xScale, yScale;
		float angle;

		mapFile >> xPos;
		mapFile >> yPos;
		mapFile >> xScale;
		mapFile >> yScale;
		mapFile >> angle;

		ActorFactory::Factory().spawnActor(className, world, Vector2D(xPos, yPos),
			Vector2D(xScale, yScale), Rotator(angle));
	}
	mapFile.close();

	// loading paths
	typedef std::map<int, PathPoint::Ptr> PathPointMap;
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
		points.at(point1)->connect(points.at(point2).get());
	}

	// put paths into the world
	for (int i = 0, pointsSize = static_cast<int>(points.size()); i < pointsSize; i++)
	{
		world->addPathPoint(points.at(i));
	}

	pathsFile.close();
}
