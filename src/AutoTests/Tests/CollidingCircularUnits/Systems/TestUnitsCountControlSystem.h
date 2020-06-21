#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class TestUnitsCountControlSystem : public System
{
public:
	TestUnitsCountControlSystem(WorldHolder& worldHolder);

	void update() override;
	std::string getName() override { return "TestUnitsCountControlSystem"; }

private:
private:
	WorldHolder& mWorldHolder;
	int mTicksPassed = 0;
};
