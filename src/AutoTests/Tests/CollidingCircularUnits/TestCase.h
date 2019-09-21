#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Application/ArgumentsParser.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

#include "AutoTests/BaseTestCase.h"

class CollidingCircularUnitsTestCase : public BaseTestCase
{
public:
	using BaseTestCase::BaseTestCase;

	void initTestCase(const ArgumentsParser& arguments) override;
	void initResources() override {}
	void setKeyboardKeyState(int, bool) override {}
	void setMouseKeyState(int, bool) override {}
};
