#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"
#include "GameData/World.h"
#include "Utils/Application/ArgumentsParser.h"

#include "HAL/GameBase.h"
#include "HAL/KeyStatesMap.h"

class Game : public HAL::GameBase
{
public:
	void start(ArgumentsParser& arguments);
	void update(float dt) override;
	void setKeyState(int key, bool isPressed) override;

private:
	World mWorld;
	HAL::KeyStatesMap mKeyStates;
	SystemsManager mSystemsManager;
	ComponentFactory mComponentFactory;
};
