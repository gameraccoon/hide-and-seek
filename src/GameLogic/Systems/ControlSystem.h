#pragma once

#include <unordered_map>

#include "ECS/System.h"
#include "HAL/Base/Engine.h"

#include "HAL/KeyStatesMap.h"

/**
 * System that handles movement controls
 */
class ControlSystem : public System
{
public:
	ControlSystem(HAL::Engine* engine, HAL::KeyStatesMap* keyStates);
	~ControlSystem() override = default;

	void update(World* world, float dt) override;

private:
	HAL::Engine* mEngine;
	HAL::KeyStatesMap* mKeyStates;
};
