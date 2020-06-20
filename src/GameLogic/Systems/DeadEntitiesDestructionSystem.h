#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"


/**
 * System that destructs enemies marked with DeathComponent
 */
class DeadEntitiesDestructionSystem : public System
{
public:
	DeadEntitiesDestructionSystem(WorldHolder& worldHolder);
	~DeadEntitiesDestructionSystem() override = default;

	void update() override;
	std::string getName() override { return "DeadEnemiesDestructionSystem"; }

private:
	WorldHolder& mWorldHolder;
};
