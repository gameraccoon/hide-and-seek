#pragma once

#include <Base/ResourceManager.h>
#include <Graphics/Texture.h>

class DebugDrawer
{
public:
	DebugDrawer(std::shared_ptr<SystemInterface::ResourceManager> resourceManager);

	void render(class World* world, const class Vector2D& drawShift);

private:
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;

	Graphics::Texture mCollisionTexture;

	bool mDrawCollision = true;
};
