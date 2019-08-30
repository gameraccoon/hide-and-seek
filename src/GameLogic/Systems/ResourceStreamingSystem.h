#pragma once

#include <memory>

#include "ECS/System.h"

#include "HAL/Base/ResourceManager.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

/**
 * System that loads and distributes resources
 */
class ResourceStreamingSystem : public System
{
public:
	ResourceStreamingSystem(WorldHolder& worldHolder, HAL::ResourceManager* resourceManager);
	~ResourceStreamingSystem() override = default;

	void update() override;

private:
	WorldHolder& mWorldHolder;
	HAL::ResourceManager* mResourceManager;
};
