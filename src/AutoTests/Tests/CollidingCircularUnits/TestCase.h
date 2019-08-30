#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"

#include "GameData/World.h"

#include "Utils/Application/ArgumentsParser.h"

#include "HAL/GameBase.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

class CollidingCircularUnitsTestCase : public HAL::GameBase
{
public:
	using HAL::GameBase::GameBase;

	void start(ArgumentsParser& arguments);
	void update(float dt) override;
	void initResources() override {}
	void setKeyboardKeyState(int, bool) override {}
	void setMouseKeyState(int, bool) override {}

private:
	WorldHolder mWorldHolder;
	World mWorld;
	TimeData mTime;
	SystemsManager mSystemsManager;
	ComponentFactory mComponentFactory;
	int ticksCount = 0;
};
