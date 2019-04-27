#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>
#include <Components/CameraComponent.h>
#include <EngineFwd.h>

#ifdef DEBUG
#include <Graphics/DebugDrawer.h>
#endif // DEBUG

/**
 * System that loads and distributes resources
 */
class ResourceStreamingSystem : public System
{
public:
	ResourceStreamingSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager);
	virtual ~ResourceStreamingSystem() override = default;

	virtual void update(World* world, float dt) override;

private:
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;
};