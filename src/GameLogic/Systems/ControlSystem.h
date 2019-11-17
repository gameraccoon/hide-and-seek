#pragma once

#include <unordered_map>

#include "ECS/System.h"
#include "HAL/Base/Engine.h"

#include "HAL/KeyStatesMap.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that handles movement controls
 */
class ControlSystem : public System
{
public:
	ControlSystem(WorldHolder& worldHolder, HAL::Engine& engine, HAL::KeyStatesMap& keyStates);
	~ControlSystem() override = default;

	void update() override;
	std::string getName() override { return "ControlSystem"; }

private:
	WorldHolder& mWorldHolder;
	HAL::Engine& mEngine;
	HAL::KeyStatesMap& mKeyStates;
};
