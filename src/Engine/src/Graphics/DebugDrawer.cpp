#include "Graphics/DebugDrawer.h"

#include <Core/World.h>
#include <Components/CollisionComponent.h>

DebugDrawer::DebugDrawer(std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
	, mCollisionTexture(resourceManager->getTexture("resources/textures/colision.png"))
{

}

void DebugDrawer::render(World* world, const Vector2D& drawShift)
{
	if (mDrawCollision)
	{
		world->getEntityManger().forEachEntity<CollisionComponent>([collisionTexture = mCollisionTexture, drawShift](CollisionComponent* collisionComponent)
		{
			collisionTexture.draw(collisionComponent->getBoundingBox().minX + drawShift.x,
								  collisionComponent->getBoundingBox().minY + drawShift.y,
								  0.0f, 0.0f,
								  (collisionComponent->getBoundingBox().maxX - collisionComponent->getBoundingBox().minX) / collisionTexture.getWidth(),
								  (collisionComponent->getBoundingBox().maxY - collisionComponent->getBoundingBox().minY) / collisionTexture.getHeight());
			return true;
		});
	}
}
