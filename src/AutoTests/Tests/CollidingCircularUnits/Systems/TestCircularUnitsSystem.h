#pragma once

#include <memory>
#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class TestCircularUnitsSystem : public System
{
public:
	TestCircularUnitsSystem(WorldHolder& worldHolder);
	~TestCircularUnitsSystem() override = default;

	void update() override;
	std::string getName() override { return "TestCircularUnitsSystem"; }

private:
	WorldHolder& mWorldHolder;
};
