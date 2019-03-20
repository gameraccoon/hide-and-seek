#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>
#include <Components/CameraComponent.h>

/**
 * System that handles rendering of world objects
 */
class RenderSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	RenderSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager);
	virtual ~RenderSystem() override = default;

	virtual void update(World* world, float dt) override;

private:
	std::weak_ptr<SystemInterface::ResourceManager> mResourceManager;
};
