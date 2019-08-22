#pragma once

#include <memory>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/TimeData.h"

/**
 * System that updates animations
 */
class AnimationSystem : public System
{
public:
	AnimationSystem(WorldHolder& worldHolder, const TimeData& timeData);
	~AnimationSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
};
