#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"
#include "GameData/World.h"
#include "Utils/Application/ArgumentsParser.h"

#include "HAL/GameBase.h"

class CollidingCircularUnitsTestCase : public HAL::GameBase
{
public:
	void start(ArgumentsParser& arguments);
	void update(float dt) override;
	void setKeyState(int, bool) override {}

private:
	World mWorld;
	SystemsManager mSystemsManager;
	ComponentFactory mComponentFactory;
	int ticksCount = 0;
};
