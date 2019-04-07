#pragma once

#include <Core/World.h>
#include <Base/ResourceManager.h>
#include <Graphics/Texture.h>

class DebugDrawer
{
public:
	DebugDrawer(std::shared_ptr<SystemInterface::ResourceManager> resourceManager);

	void render(World* world, const Vector2D& drawShift);

private:
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;

	Graphics::Texture mCollisionTexture;

	bool mDrawCollision = false;
};
