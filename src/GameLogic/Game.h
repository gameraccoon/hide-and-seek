#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Application/ArgumentsParser.h"

#include "HAL/GameBase.h"
#include "HAL/KeyStatesMap.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

class Game : public HAL::GameBase
{
public:
	void start(ArgumentsParser& arguments);
	void update(float dt) override;
	void setKeyState(int key, bool isPressed) override;

private:
	void initSystems();
	void initResources();

private:
	WorldHolder mWorldHolder;
	World mWorld;
	GameData mGameData;

	HAL::KeyStatesMap mKeyStates;
	SystemsManager mSystemsManager;
	ComponentFactory mComponentFactory;
	TimeData mTime;
};
