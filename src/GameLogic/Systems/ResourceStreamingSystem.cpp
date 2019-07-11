#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/RenderComponent.generated.h"

#include "GameData/World.h"

ResourceStreamingSystem::ResourceStreamingSystem(std::shared_ptr<HAL::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update(World* world, float /*dt*/)
{
	world->getEntityManger().forEachComponentSet<RenderComponent>([&resourceManager = mResourceManager](RenderComponent* renderComponent)
	{
		if (ResourceHandle textureHandle = renderComponent->getSpriteHandle(); !textureHandle.isValid())
		{
			renderComponent->setSpriteHandle(resourceManager->lockSprite(renderComponent->getTexturePath()));
		}
	});
}
