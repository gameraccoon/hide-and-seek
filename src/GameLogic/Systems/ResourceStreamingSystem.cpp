#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/RenderComponent.generated.h"

#include "GameData/World.h"


ResourceStreamingSystem::ResourceStreamingSystem(WorldHolder& worldHolder, std::shared_ptr<HAL::ResourceManager> resourceManager)
	: mWorldHolder(worldHolder)
	, mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update()
{
	World* world = mWorldHolder.world;

	world->getEntityManger().forEachComponentSet<RenderComponent>([&resourceManager = mResourceManager](RenderComponent* renderComponent)
	{
		if (ResourceHandle textureHandle = renderComponent->getSpriteHandle(); !textureHandle.isValid())
		{
			renderComponent->setSpriteHandle(resourceManager->lockSprite(renderComponent->getTexturePath()));
		}
	});
}
