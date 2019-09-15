#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that resolve object collisions
 */
class CollisionSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	CollisionSystem(WorldHolder& worldHolder);
	~CollisionSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
};
