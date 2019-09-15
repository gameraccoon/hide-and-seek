#pragma once

#include <memory>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that calculates AI
 */
class AiSystem : public System
{
public:
	AiSystem(WorldHolder& worldHolder);
	~AiSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
};