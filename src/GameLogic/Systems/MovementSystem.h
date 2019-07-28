#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "HAL/Base/Engine.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that process characters and objects movement
 */
class MovementSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	MovementSystem(WorldHolder& worldHolder, const TimeData& timeData);
	~MovementSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
};
