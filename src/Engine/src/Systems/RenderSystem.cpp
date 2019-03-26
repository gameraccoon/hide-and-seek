#include "Systems/RenderSystem.h"

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>
#include <Base/Engine.h>

RenderSystem::RenderSystem(SystemInterface::Engine* engine, std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mEngine(engine)
	, mResourceManager(resourceManager)
{
}

void RenderSystem::update(World* world, float /*dt*/)
{
	auto sharedResourceManager = mResourceManager.lock();
	if (sharedResourceManager == nullptr)
	{
		return;
	}

	NullableEntity mainCamera = world->getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	TransformComponent::Ptr cameraTransformComponent = std::get<0>(world->getEntityComponents<TransformComponent>(mainCamera.getEntity()));
	if (cameraTransformComponent == nullptr)
	{
			return;
	}

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D screenHalfSize = Vector2D(mEngine->getWidth(), mEngine->getHeight()) * 0.5f;

	world->forEachEntity(std::function<bool(std::tuple<std::shared_ptr<RenderComponent>, std::shared_ptr<TransformComponent>>)>([&sharedResourceManager, &cameraLocation, &screenHalfSize](std::tuple<std::shared_ptr<RenderComponent>, std::shared_ptr<TransformComponent>> components) -> bool {
		auto renderComponent = std::get<0>(components);
		auto transformComponent = std::get<1>(components);

		std::string texturePath = renderComponent->getTexturePath();

		if (texturePath.empty())
		{
			return true;
		}

		Graphics::Texture texure = sharedResourceManager->getTexture(texturePath);

		auto location = transformComponent->getLocation() - cameraLocation + screenHalfSize;
		auto anchor = renderComponent->getAnchor();
		texure.draw(location.x, location.y, anchor.x, anchor.y, transformComponent->getRotation().getValue(), 1.0f);
		return true;
	}));
}
