#ifndef LEVEL_LOADER
#define LEVEL_LOADER

#include "../src/World.h"

class LevelLoader
{
public:
	LevelLoader(void);
	~LevelLoader(void);

	static void save(World* world, const std::string levelName);
	static void load(World* world, const std::string levelName);
};

#endif
