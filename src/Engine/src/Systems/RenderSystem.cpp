#include "Systems/RenderSystem.h"

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>

RenderSystem::RenderSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
{
}

void RenderSystem::update(World* world, float /*dt*/)
{
	auto sharedResourceManager = mResourceManager.lock();
	if (sharedResourceManager == nullptr)
	{
		return;
	}

//	NullableEntity mainCamera = world->getMainCamera();
//	if (mainCamera.isNull())
//	{
//		return;
//	}

//	TransformComponent::Ptr cameraTransformComponent = std::get<0>(world->getEntityComponents<TransformComponent>(mainCamera.getEntity()));
//	if (cameraTransformComponent == nullptr)
//	{
//			return;
//	}

	//auto cameraLocation = cameraTransformComponent->getLocation();

	world->forEachEntity<RenderComponent, TransformComponent>(std::function<bool(std::tuple<std::shared_ptr<RenderComponent>, std::shared_ptr<TransformComponent>>)>([&sharedResourceManager/*, &cameraLocation*/](std::tuple<std::shared_ptr<RenderComponent>, std::shared_ptr<TransformComponent>> components) -> bool {
		auto renderComponent = std::get<0>(components);
		auto transformComponent = std::get<1>(components);

		std::string texturePath = renderComponent->getTexturePath();

		if (texturePath.empty())
		{
			return true;
		}

		Graphics::Texture texure = sharedResourceManager->getTexture(texturePath);

		auto location = transformComponent->getLocation();// - cameraLocation;
		texure.draw(location.x, location.y, transformComponent->getRotation().getValue(), 1.0f);
		return true;
	}));
}
