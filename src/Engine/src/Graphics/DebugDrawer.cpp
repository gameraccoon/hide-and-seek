#include "Graphics/DebugDrawer.h"

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
		world->forEachEntity<CollisionComponent>([collisionTexture = mCollisionTexture, drawShift](std::tuple<CollisionComponent::Ptr>& component)
		{
			collisionTexture.draw(std::get<0>(component)->getBoundingBox().minX + drawShift.x,
								  std::get<0>(component)->getBoundingBox().minY + drawShift.y,
								  0.0f, 0.0f,
								  (std::get<0>(component)->getBoundingBox().maxX - std::get<0>(component)->getBoundingBox().minX) / collisionTexture.getWidth(),
								  (std::get<0>(component)->getBoundingBox().maxY - std::get<0>(component)->getBoundingBox().minY) / collisionTexture.getHeight());
			return true;
		});
	}
}
