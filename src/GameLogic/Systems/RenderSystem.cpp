#include "GameLogic/Systems/RenderSystem.h"

#include <algorithm>

#include "GameData/Components/SpriteComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/VisibilityPolygon.h"

#include "HAL/Base/Math.h"
#include "HAL/Base/Engine.h"
#include "HAL/Internal/SdlSurface.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>


RenderSystem::RenderSystem(WorldHolder& worldHolder, HAL::Engine* engine, HAL::ResourceManager* resourceManager)
	: mWorldHolder(worldHolder)
	, mEngine(engine)
	, mResourceManager(resourceManager)
{
	mLightSpriteHandle = resourceManager->lockSprite("resources/textures/light.png");
}

void RenderSystem::update()
{
	World* world = mWorldHolder.world;
	Graphics::Renderer* renderer = mEngine->getRenderer();

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
		world->getEntityManger().forEachComponentSet<SpriteComponent, TransformComponent>([&drawShift, &resourceManager = mResourceManager, renderer](SpriteComponent* sprite, TransformComponent* transform)
		{
			ResourceHandle spriteHandle = sprite->getSpriteHandle();
			if (spriteHandle.isValid())
			{
				const Graphics::Sprite& spriteData = resourceManager->getResource<Graphics::Sprite>(spriteHandle);
				if (spriteData.isValid())
				{
					auto location = transform->getLocation() + drawShift;
					auto anchor = sprite->getAnchor();
					auto size = sprite->getSize();
					renderer->render(*spriteData.getTexture(), location, size, anchor, transform->getRotation().getValue(), spriteData.getUV(), 1.0f);
				}
			}
		});
	}
}

void RenderSystem::drawVisibilityPolygon(const Graphics::Sprite& lightSprite, const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift)
{
	if (polygon.size() > 2)
	{
		Graphics::QuadUV quadUV = lightSprite.getUV();

		std::vector<Graphics::DrawPoint> drawablePolygon;
		drawablePolygon.reserve(polygon.size() + 2);
		drawablePolygon.push_back(Graphics::DrawPoint{ZERO_VECTOR, Graphics::QuadLerp(quadUV, 0.5f, 0.5f)});
		for (auto& point : polygon)
		{
			drawablePolygon.push_back(Graphics::DrawPoint{point, Graphics::QuadLerp(quadUV, 0.5f-point.x/fowSize.x, 0.5f-point.y/fowSize.y)});
		}
		drawablePolygon.push_back(Graphics::DrawPoint{polygon[0], Graphics::QuadLerp(quadUV, 0.5f-polygon[0].x/fowSize.x, 0.5f-polygon[0].y/fowSize.y)});

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
		mEngine->getRenderer()->renderFan(*lightSprite.getTexture(), drawablePolygon, transform, 0.5f);
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
	const Graphics::Sprite& lightSprite = mResourceManager->getResource<Graphics::Sprite>(mLightSpriteHandle);
	if (!lightSprite.isValid())
	{
		return;
	}

	const auto collidableComponents = world->getEntityManger().getComponents<CollisionComponent, TransformComponent>();
	VisibilityPolygonCalculator visibilityPolygonCalculator;

	std::vector<Vector2D> polygon;
	// draw player visibility polygon
	Vector2D playerSightPosition = GetPlayerSightPosition(world);
	visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, playerSightPosition, maxFov);
	drawVisibilityPolygon(lightSprite, polygon, maxFov, drawShift + playerSightPosition);

	// ToDo: we calculate visibility polygon for every light source in the each frame to
	// be able to work with worst-case scenario as long as possible
	// optimizations such as dirty flag and spatial hash are on the way to be impelemnted
	// draw light
	world->getEntityManger().forEachComponentSet<LightComponent, TransformComponent>([&collidableComponents, &visibilityPolygonCalculator, maxFov, &drawShift, &lightSprite, &polygon, this](LightComponent* /*light*/, TransformComponent* transform)
	{
		visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, transform->getLocation(), maxFov);
		drawVisibilityPolygon(lightSprite, polygon, maxFov, drawShift + transform->getLocation());
	});
}
