#include "Systems/ResourceStreamingSystem.h"

#include <Components/RenderComponent.h>

ResourceStreamingSystem::ResourceStreamingSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update(World* world, float /*dt*/)
{
	world->getEntityManger().forEachEntity<RenderComponent>([&resourceManager = mResourceManager](RenderComponent* renderComponent)
	{
		const auto& optionalTexture = renderComponent->getTexture();
		if (!optionalTexture.has_value())
		{
			renderComponent->setTexture(resourceManager->getTexture(renderComponent->getTexturePath()));
			renderComponent->calcScaleFromSize();
		}
	});
}
