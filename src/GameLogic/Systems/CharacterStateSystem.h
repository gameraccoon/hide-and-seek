#pragma once

#include <memory>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that ensures correct character state
 */
class CharacterStateSystem : public System
{
public:
	CharacterStateSystem(WorldHolder& worldHolder);
	~CharacterStateSystem() override = default;

	void update() override;
	std::string getName() override { return "CharacterStateSystem"; }

private:
	WorldHolder& mWorldHolder;
};
