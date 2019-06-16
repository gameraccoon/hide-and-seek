#pragma once

#include <unordered_map>

#include <Core/System.h>
#include <Base/Engine.h>

#include <KeyStatesMap.h>

class World;

/**
 * System that handles movement controls
 */
class ControlSystem : public System
{
public:
	ControlSystem(SystemInterface::Engine* engine, SystemInterface::KeyStatesMap* keyStates);
	~ControlSystem() override = default;

	void update(World* world, float dt) override;

private:
	SystemInterface::Engine* mEngine;
	SystemInterface::KeyStatesMap* mKeyStates;
};
