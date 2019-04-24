#include "Graphics/DebugDrawer.h"

#include <Core/World.h>
#include <Components/CollisionComponent.h>
#include <Base/Engine.h>

DebugDrawer::DebugDrawer(std::shared_ptr<SystemInterface::ResourceManager> resourceManager, SystemInterface::Engine* engine)
	: mEngine(engine)
	, mResourceManager(resourceManager)
	, mCollisionTextureHandle(resourceManager->lockTexture("resources/textures/colision.png"))
{
}

void DebugDrawer::render(World* world, const Vector2D& drawShift)
{
	if (mDrawCollision)
	{
		const Graphics::Texture& collisionTexture = mResourceManager->getTexture(mCollisionTextureHandle);
		world->getEntityManger().forEachEntity<CollisionComponent>([collisionTexture, drawShift, engine = mEngine](CollisionComponent* collisionComponent)
		{
			engine->render(collisionTexture.getSurface(),
				collisionComponent->getBoundingBox().minX + drawShift.x,
				collisionComponent->getBoundingBox().minY + drawShift.y,
				0.0f, 0.0f,
				(collisionComponent->getBoundingBox().maxX - collisionComponent->getBoundingBox().minX) / collisionTexture.getWidth(),
				(collisionComponent->getBoundingBox().maxY - collisionComponent->getBoundingBox().minY) / collisionTexture.getHeight());
			return true;
		});
	}
}
