#pragma once

#include <Core/System.h>
#include <Base/Engine.h>

/**
 * System that handles movement controls
 */
class ControlSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	ControlSystem(SystemInterface::Engine* engine, KeyStatesMap* keyStatesMap);
	virtual ~ControlSystem() override = default;

	virtual void update(World* world, float dt) override;

private:
	SystemInterface::Engine* mEngine;
	KeyStatesMap* mKeyStatesMap;
};
