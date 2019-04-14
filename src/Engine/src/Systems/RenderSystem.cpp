#include "Systems/RenderSystem.h"

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>
#include <Base/Engine.h>

RenderSystem::RenderSystem(SystemInterface::Engine* engine, std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mEngine(engine)
#ifdef DEBUG
	, mDebugDrawer(resourceManager)
#endif // DEBUG
{
}

void RenderSystem::update(World* world, float /*dt*/)
{
	NullableEntity mainCamera = world->getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	auto [cameraTransformComponent] = world->getEntityManger().getEntityComponents<TransformComponent>(mainCamera.getEntity());
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());
	Vector2D screenHalfSize = Vector2D(mEngine->getWidth(), mEngine->getHeight()) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation + (screenHalfSize - mouseScreenPos) * 0.5;

	world->getEntityManger().forEachEntity<RenderComponent, TransformComponent>([&drawShift](RenderComponent* renderComponent, TransformComponent* transformComponent)
	{
		const auto& optionalTexture = renderComponent->getTexture();
		if (optionalTexture.has_value())
		{
			if (const Graphics::Texture& texture = optionalTexture.value(); texture.isValid())
			{
				auto location = transformComponent->getLocation() + drawShift;
				auto anchor = renderComponent->getAnchor();
				auto scale = renderComponent->getScale();
				texture.draw(location.x, location.y, anchor.x, anchor.y, scale.x, scale.y, transformComponent->getRotation().getValue(), 1.0f);
			}
		}
	});

#ifdef DEBUG
	mDebugDrawer.render(world, drawShift);
#endif // DEBUG
}
