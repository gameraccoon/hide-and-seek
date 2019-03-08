#include "Systems/RenderSystem.h"

#include <Components/RenderComponent.h>

RenderSystem::RenderSystem(std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mResourceManager(resourceManager)
{

}

void RenderSystem::update(World* world, float /*dt*/)
{
	auto mainCameraComponent = getMainCameraComponent(world);
	if (mainCameraComponent == nullptr)
	{
		return;
	}

	auto cameraTransformComponent = mainCameraComponent->getTransformComponent().lock();
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	//auto cameraLocation = cameraTransformComponent->getLocation();

	auto sharedResourceManager = mResourceManager.lock();
	if (sharedResourceManager == nullptr)
	{
		return;
	}

	auto renderComponents = world->getComponents<RenderComponent>();
	for (auto& renderComponent : renderComponents)
	{
		auto transformComponent = renderComponent->getTransformComponent().lock();
		if (transformComponent == nullptr)
		{
			continue;
		}

		std::string texturePath = renderComponent->getTexturePath();

		if (texturePath.empty())
		{
			continue;
		}

		Graphics::Texture texure = sharedResourceManager->getTexture(texturePath);

		auto location = transformComponent->getLocation() /*- cameraLocation*/;
		texure.draw(location.x, location.y, transformComponent->getRotation().getValue(), 1.0f);
	}
}

std::shared_ptr<CameraComponent> RenderSystem::getMainCameraComponent(World* world)
{
	std::weak_ptr<CameraComponent> mainCameraComponent = world->getMainCamera();

	if (auto lockedCameraComponent = mainCameraComponent.lock())
	{
		return lockedCameraComponent;
	}

	return nullptr;
}
