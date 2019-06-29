#pragma once

#include <memory>
#include "ECS/System.h"
#include "HAL/Base/ResourceManager.h"

/**
 * System that loads and distributes resources
 */
class ResourceStreamingSystem : public System
{
public:
	ResourceStreamingSystem(std::shared_ptr<HAL::ResourceManager> resourceManager);
	~ResourceStreamingSystem() override = default;

	void update(World* world, float dt) override;

private:
	std::shared_ptr<HAL::ResourceManager> mResourceManager;
};
