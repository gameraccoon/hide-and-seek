#ifndef LEVEL_LOADER
#define LEVEL_LOADER

#include "../src/World.h"

class LevelLoader
{
public:
	LevelLoader(void);
	~LevelLoader(void);

	static void Save(World* world, const std::string levelName);
	static void Load(World* world, const std::string levelName);
};

#endif