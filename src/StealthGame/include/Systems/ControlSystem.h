#pragma once

#include <unordered_map>

#include <Core/System.h>
#include <Base/Engine.h>

class World;

/**
 * System that handles movement controls
 */
class ControlSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	ControlSystem(SystemInterface::Engine* engine, KeyStatesMap* keyStatesMap);
	~ControlSystem() override = default;

	void update(World* world, float dt) override;

private:
	SystemInterface::Engine* mEngine;
	KeyStatesMap* mKeyStatesMap;
};
