#pragma once

#include <EngineFwd.h>
#include <Base/ResourceManager.h>
#include <Structures/ResourceHandle.h>

class DebugDrawer
{
public:
	DebugDrawer(std::shared_ptr<SystemInterface::ResourceManager> resourceManager, SystemInterface::Engine* engine);

	void render(class World* world, const class Vector2D& drawShift);

private:
	SystemInterface::Engine* mEngine;
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;
	ResourceHandle mCollisionTextureHandle;

	bool mDrawCollision = false;
};
