#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "HAL/Base/Engine.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that handles camera position
 */
class CameraSystem : public System
{
public:
	CameraSystem(WorldHolder& worldHolder, HAL::Engine& engine);
	~CameraSystem() override = default;

	void update() override;
	std::string getName() override { return "CameraSystem"; }

private:
	WorldHolder& mWorldHolder;
	HAL::Engine& mEngine;
};
