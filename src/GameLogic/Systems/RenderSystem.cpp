#include "GameLogic/Systems/RenderSystem.h"

#include <algorithm>

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/VisibilityPolygon.h"

#include "HAL/Base/Engine.h"
#include "HAL/Internal/SdlSurface.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::RenderSystem(HAL::Engine* engine, const std::shared_ptr<HAL::ResourceManager>& resourceManager)
	: mEngine(engine)
	, mResourceManager(resourceManager)
{
}

void RenderSystem::update(World* world, float /*dt*/)
{
	static const Vector2D maxFov(500.0f, 500.0f);

	OptionalEntity mainCamera = world->getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	auto [cameraTransformComponent] = world->getEntityManger().getEntityComponents<TransformComponent>(mainCamera.getEntity());
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	auto [renderMode] = world->getWorldComponents().getComponents<RenderModeComponent>();

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());
	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine->getWidth()), static_cast<float>(mEngine->getHeight())) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation + (screenHalfSize - mouseScreenPos) * 0.5;

	if (!renderMode || renderMode->getIsDrawLightsEnabled())
	{
		drawLights(world, drawShift, maxFov);
	}

	if (!renderMode || renderMode->getIsDrawVisibleEntitiesEnabled())
	{
		world->getEntityManger().forEachComponentSet<RenderComponent, TransformComponent>([&drawShift, &resourceManager = mResourceManager, engine = mEngine](RenderComponent* renderComponent, TransformComponent* transformComponent)
		{
			ResourceHandle textureHandle = renderComponent->getTextureHandle();
			if (textureHandle.isValid())
			{
				const Graphics::Texture& texture = resourceManager->getTexture(textureHandle);
				if (texture.isValid())
				{
					auto location = transformComponent->getLocation() + drawShift;
					auto anchor = renderComponent->getAnchor();
					auto size = renderComponent->getSize();
					engine->render(texture.getSurface(), location.x, location.y, size.x, size.y, anchor.x, anchor.y, transformComponent->getRotation().getValue(), 1.0f);
				}
			}
		});
	}
}

void RenderSystem::drawVisibilityPolygon(const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift)
{
	if (polygon.size() > 2)
	{
		std::vector<HAL::DrawPoint> drawablePolygon;
		drawablePolygon.reserve(polygon.size() + 2);
		drawablePolygon.push_back(HAL::DrawPoint{ZERO_VECTOR, Vector2D(0.5f, 0.5f)});
		for (auto& point : polygon)
		{
			drawablePolygon.push_back(HAL::DrawPoint{point, Vector2D(0.5f-point.x/fowSize.x, 0.5f-point.y/fowSize.y)});
		}
		drawablePolygon.push_back(HAL::DrawPoint{polygon[0], Vector2D(0.5f-polygon[0].x/fowSize.x, 0.5f-polygon[0].y/fowSize.y)});

		static HAL::Internal::SdlSurface surface("resources/textures/light.png");
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
		mEngine->renderFan(&surface, drawablePolygon, transform, 0.5f);
	}
}

Vector2D RenderSystem::GetPlayerSightPosition(World* world)
{
	Vector2D result;

	if (OptionalEntity playerEntity = world->getPlayerControlledEntity(); playerEntity.isValid())
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
	world->getEntityManger().forEachComponentSet<LightComponent, TransformComponent>([&collidableComponents, &visibilityPolygonCalculator, maxFov, &drawShift, &polygon, this](LightComponent* /*light*/, TransformComponent* transform)
	{
		visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, transform->getLocation(), maxFov);
		drawVisibilityPolygon(polygon, maxFov, drawShift + transform->getLocation());
	});
}
