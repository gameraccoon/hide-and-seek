#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>

/**
 * System that loads and distributes resources
 */
class ResourceStreamingSystem : public System
{
public:
	ResourceStreamingSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager);
	~ResourceStreamingSystem() override = default;

	void update(World* world, float dt) override;

private:
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;
};
