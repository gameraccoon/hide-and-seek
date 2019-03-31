#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>
#include <Components/CameraComponent.h>
#include <EngineFwd.h>

/**
 * System that handles rendering of world objects
 */
class RenderSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	RenderSystem(SystemInterface::Engine* engine);
	virtual ~RenderSystem() override = default;

	virtual void update(World* world, float dt) override;

private:
	SystemInterface::Engine* mEngine;
};
