#include "Systems/ResourceStreamingSystem.h"

#include <Components/RenderComponent.generated.h>

ResourceStreamingSystem::ResourceStreamingSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
{
}

static void updateTextureScaleFromSize(RenderComponent* renderComponent, const Graphics::Texture& texture)
{
	if (texture.isValid())
	{
		Vector2D size = renderComponent->getSize();
		if (size != ZERO_VECTOR)
		{
			renderComponent->setScale(Vector2D(size.x / texture.getWidth(), size.y / texture.getHeight()));
		}
	}
}

void ResourceStreamingSystem::update(World* world, float /*dt*/)
{
	world->getEntityManger().forEachEntity<RenderComponent>([&resourceManager = mResourceManager](RenderComponent* renderComponent)
	{
		if (ResourceHandle textureHandle = renderComponent->getTextureHandle(); !textureHandle.isValid())
		{
			renderComponent->setTextureHandle(resourceManager->lockTexture(renderComponent->getTexturePath()));
			updateTextureScaleFromSize(renderComponent, resourceManager->getTexture(renderComponent->getTextureHandle()));
		}
	});
}
