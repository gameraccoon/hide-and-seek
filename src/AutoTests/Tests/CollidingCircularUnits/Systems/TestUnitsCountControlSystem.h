#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class TestUnitsCountControlSystem : public System
{
public:
	TestUnitsCountControlSystem(WorldHolder& worldHolder);

	void update() override;

private:
private:
	WorldHolder& mWorldHolder;
	int ticksPassed = 0;
};
