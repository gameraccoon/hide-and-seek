#include "Systems/RenderSystem.h"

#include <algorithm>

#include "Components/RenderComponent.generated.h"
#include "Components/TransformComponent.generated.h"
#include "Components/CollisionComponent.generated.h"
#include "Components/LightComponent.generated.h"
#include "Base/Engine.h"
#include "Utils/Geometry/VisibilityPolygon.h"
#include "Core/World.h"

// for tests only
#include "../src/Internal/SdlSurface.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::RenderSystem(SystemInterface::Engine* engine, __attribute__((unused)) const std::shared_ptr<SystemInterface::ResourceManager>& resourceManager)
	: mEngine(engine)
#ifdef DEBUG
	, mResourceManager(resourceManager)
	, mDebugDrawer(resourceManager, engine)
#endif // DEBUG
{
}

void RenderSystem::update(World* world, float /*dt*/)
{
	static const Vector2D maxFov(500.0f, 500.0f);

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
	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine->getWidth()), static_cast<float>(mEngine->getHeight())) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation + (screenHalfSize - mouseScreenPos) * 0.5;

	drawLights(world, drawShift, maxFov);

	world->getEntityManger().forEachEntity<RenderComponent, TransformComponent>([&drawShift, &resourceManager = mResourceManager, engine = mEngine](RenderComponent* renderComponent, TransformComponent* transformComponent)
	{
		ResourceHandle textureHandle = renderComponent->getTextureHandle();
		if (textureHandle.isValid())
		{
			const Graphics::Texture& texture = resourceManager->getTexture(textureHandle);
			if (texture.isValid())
			{
				auto location = transformComponent->getLocation() + drawShift;
				auto anchor = renderComponent->getAnchor();
				auto scale = renderComponent->getScale();
				engine->render(texture.getSurface(), location.x, location.y, anchor.x, anchor.y, scale.x, scale.y, transformComponent->getRotation().getValue(), 1.0f);
			}
		}
	});

#ifdef DEBUG
	mDebugDrawer.render(world, drawShift);
#endif // DEBUG
}

void RenderSystem::drawVisibilityPolygon(const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift)
{
	if (polygon.size() > 2)
	{
		std::vector<SystemInterface::DrawPoint> drawablePolygon;
		drawablePolygon.reserve(polygon.size() + 2);
		drawablePolygon.push_back(SystemInterface::DrawPoint{ZERO_VECTOR, Vector2D(0.5f, 0.5f)});
		for (auto& point : polygon)
		{
			drawablePolygon.push_back(SystemInterface::DrawPoint{point, Vector2D(0.5f-point.x/fowSize.x, 0.5f-point.y/fowSize.y)});
		}
		drawablePolygon.push_back(SystemInterface::DrawPoint{polygon[0], Vector2D(0.5f-polygon[0].x/fowSize.x, 0.5f-polygon[0].y/fowSize.y)});

		static SystemInterface::Internal::SdlSurface surface("resources/textures/light.png");
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
		mEngine->render(&surface, drawablePolygon, transform, 0.5f);
	}
}

Vector2D RenderSystem::GetPlayerSightPosition(World* world)
{
	Vector2D result;

	if (NullableEntity playerEntity = world->getPlayerControlledEntity(); playerEntity.isValid())
	{
		auto [playerTransform] = world->getEntityManger().getEntityComponents<TransformComponent>(playerEntity.getEntity());

		if (playerTransform != nullptr)
		{
			result = playerTransform->getLocation();
		}
	}

	return result;
}

void RenderSystem::drawLights(World* world, const Vector2D& drawShift, const Vector2D& maxFov)
{
	const auto collidableComponents = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();
	VisibilityPolygonCalculator visibilityPolygonCalculator;

	std::vector<Vector2D> polygon;
	// draw player visibility polygon
	Vector2D playerSightPosition = GetPlayerSightPosition(world);
	visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, playerSightPosition, maxFov);
	drawVisibilityPolygon(polygon, maxFov, drawShift + playerSightPosition);

	// ToDo: we calculate visibility polygon for every light source in the each frame to
	// be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	// draw light
	world->getEntityManger().forEachEntity<LightComponent, TransformComponent>([&collidableComponents, &visibilityPolygonCalculator, maxFov, &drawShift, &polygon, this](LightComponent* /*light*/, TransformComponent* transform)
	{
		visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, transform->getLocation(), maxFov);
		drawVisibilityPolygon(polygon, maxFov, drawShift + transform->getLocation());
	});
}
