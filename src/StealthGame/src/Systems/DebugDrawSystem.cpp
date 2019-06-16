#include "Systems/DebugDrawSystem.h"

#include "Components/RenderComponent.generated.h"
#include "Components/TransformComponent.generated.h"
#include "Components/CollisionComponent.generated.h"
#include "Components/NavMeshComponent.generated.h"
#include "Components/RenderModeComponent.generated.h"

#include "Base/Engine.h"
#include "Utils/Geometry/VisibilityPolygon.h"
#include "Core/World.h"

#include "../src/Internal/SdlSurface.h"
//#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

DebugDrawSystem::DebugDrawSystem(SystemInterface::Engine* engine, const std::shared_ptr<SystemInterface::ResourceManager>& resourceManager)
	: mEngine(engine)
	, mResourceManager(resourceManager)
{
	mCollisionTextureHandle = resourceManager->lockTexture("resources/textures/collision.png");
	mNavmeshTextureHandle = resourceManager->lockTexture("resources/textures/collision.png");
}

void DebugDrawSystem::update(World* world, float /*dt*/)
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

	auto [renderMode] = world->getWorldComponents().getComponents<RenderModeComponent>();
	if (renderMode && renderMode->getIsDrawDebugCollisionsEnabled())
	{
		const Graphics::Texture& collisionTexture = mResourceManager->getTexture(mCollisionTextureHandle);
		world->getEntityManger().forEachComponentSet<CollisionComponent>([collisionTexture, drawShift, engine = mEngine](CollisionComponent* collisionComponent)
		{
			engine->render(collisionTexture.getSurface(),
				collisionComponent->getBoundingBox().minX + drawShift.x,
				collisionComponent->getBoundingBox().minY + drawShift.y,
				0.0f, 0.0f,
				(collisionComponent->getBoundingBox().maxX - collisionComponent->getBoundingBox().minX) / collisionTexture.getWidth(),
				(collisionComponent->getBoundingBox().maxY - collisionComponent->getBoundingBox().minY) / collisionTexture.getHeight());
			return true;
		});
	}

	if (renderMode && renderMode->getIsDrawDebugNavmeshEnabled())
	{
		const Graphics::Texture& navmeshTexture = mResourceManager->getTexture(mNavmeshTextureHandle);
		world->getEntityManger().forEachComponentSet<NavMeshComponent>([navmeshTexture, drawShift, engine = mEngine](NavMeshComponent* navmeshComponent)
		{
			for (const std::array<Vector2D, 3>& tri : navmeshComponent->getNavMeshRef().mTriangles)
			{
				std::vector<SystemInterface::DrawPoint> drawablePolygon;
				drawablePolygon.push_back(SystemInterface::DrawPoint{tri[0], Vector2D(0.0f, 0.0f)});
				drawablePolygon.push_back(SystemInterface::DrawPoint{tri[1], Vector2D(1.0f, 0.0f)});
				drawablePolygon.push_back(SystemInterface::DrawPoint{tri[2], Vector2D(1.0f, 1.0f)});

				static SystemInterface::Internal::SdlSurface surface("resources/textures/light.png");
				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
				engine->render(&surface, drawablePolygon, transform, 0.5f);
			}

			return true;
		});
	}
}
