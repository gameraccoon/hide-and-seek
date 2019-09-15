#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/TimeData.h"

/**
 * System that process characters and objects movement
 */
class MovementSystem : public System
{
public:
	using KeyStatesMap = std::unordered_map<int, bool>;

public:
	MovementSystem(WorldHolder& worldHolder, const TimeData& timeData);
	~MovementSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
};
