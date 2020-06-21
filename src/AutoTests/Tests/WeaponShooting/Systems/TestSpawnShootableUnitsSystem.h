#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class TestSpawnShootableUnitsSystem : public System
{
public:
	TestSpawnShootableUnitsSystem(WorldHolder& worldHolder);

	void update() override;
	std::string getName() override { return "TestSpawnShootableUnitsSystem"; }

private:
	WorldHolder& mWorldHolder;
	int ticksPassed = 0;
};
