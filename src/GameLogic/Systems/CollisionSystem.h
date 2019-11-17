#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/TimeData.h"

/**
 * System that resolve object collisions
 */
class CollisionSystem : public System
{
public:
	using KeyStatesMap = std::unordered_map<int, bool>;

public:
	CollisionSystem(WorldHolder& worldHolder, const TimeData& timeData);
	~CollisionSystem() override = default;

	void update() override;
	std::string getName() override { return "CollisionSystem"; }

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
};
