#include "Systems/RenderSystem.h"

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>
#include <Base/Engine.h>

RenderSystem::RenderSystem(SystemInterface::Engine* engine, std::shared_ptr<SystemInterface::ResourceManager> resourceManager)
	: mEngine(engine)
	, mResourceManager(resourceManager)
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

	TransformComponent::Ptr cameraTransformComponent = std::get<0>(world->getEntityComponents<TransformComponent>(mainCamera.getEntity()));
	if (cameraTransformComponent == nullptr)
	{
			return;
	}

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D screenHalfSize = Vector2D(mEngine->getWidth(), mEngine->getHeight()) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation;

	world->forEachEntity<RenderComponent, TransformComponent>([&drawShift](std::tuple<RenderComponent::Ptr, TransformComponent::Ptr>& components) {
		auto renderComponent = std::get<0>(components);
		auto transformComponent = std::get<1>(components);

		Graphics::Texture texure = renderComponent->getTexture();

		auto location = transformComponent->getLocation() + drawShift;
		auto anchor = renderComponent->getAnchor();
		auto scale = renderComponent->getScale();
		texure.draw(location.x, location.y, anchor.x, anchor.y, scale.x, scale.y, transformComponent->getRotation().getValue(), 1.0f);

		return true;
	});

#ifdef DEBUG
	mDebugDrawer.render(world, drawShift);
#endif // DEBUG
}
