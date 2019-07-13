#pragma once

#include <memory>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that calculates AI
 */
class AiSystem : public System
{
public:
	AiSystem(WorldHolder& worldHolder, const TimeData& timeData);
	~AiSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
};
