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

private:
	WorldHolder& mWorldHolder;
};
