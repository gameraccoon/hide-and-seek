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
 * System that handles rendering of world objects
 */
class RenderSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	RenderSystem(SystemInterface::Engine* engine, std::shared_ptr<SystemInterface::ResourceManager> resourceManager);
	virtual ~RenderSystem() override = default;

	virtual void update(World* world, float dt) override;

private:
	SystemInterface::Engine* mEngine;
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;

#ifdef DEBUG
	DebugDrawer mDebugDrawer;
#endif // DEBUG
};
