#include "GameLogic/Systems/DebugDrawSystem.h"

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/World.h"

#include "HAL/Base/Engine.h"
#include "Utils/Geometry/VisibilityPolygon.h"

#include "HAL/Internal/SdlSurface.h"
#include <glm/gtc/matrix_transform.hpp>

#include <DetourNavMesh.h>

DebugDrawSystem::DebugDrawSystem(HAL::Engine* engine, const std::shared_ptr<HAL::ResourceManager>& resourceManager)
	: mEngine(engine)
	, mResourceManager(resourceManager)
{
	mCollisionTextureHandle = resourceManager->lockTexture("resources/textures/collision.png");
	mNavmeshTextureHandle = resourceManager->lockTexture("resources/textures/testTexture.png");
}

void DebugDrawSystem::update(World* world, float /*dt*/)
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
		srand(300);
		const Graphics::Texture& navMeshTexture = mResourceManager->getTexture(mNavmeshTextureHandle);
		auto [navMeshComponent] = world->getWorldComponents().getComponents<NavMeshComponent>();

		if (navMeshComponent)
		{
			if (const dtNavMesh* navmesh = navMeshComponent->getNavMeshRef().getMesh())
			{
				int maxTiles = navmesh->getMaxTiles();
				for (int i = 0; i < maxTiles; ++i)
				{
					if (const dtMeshTile* tile = navmesh->getTile(i); tile && tile->header)
					{
						std::vector<std::array<Vector2D, 3>> points;
						for (int i = 0; i < tile->header->polyCount; ++i)
						{
							const auto& poly = tile->polys[i];
							std::vector<HAL::DrawPoint> drawablePolygon;
							drawablePolygon.reserve(3);
							for (int j = 0; j < poly.vertCount; ++j)
							{
								float u = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
								float v = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

								float x = tile->verts[poly.verts[j] * 3];
								float y = tile->verts[poly.verts[j] * 3 + 2];
								drawablePolygon.push_back(HAL::DrawPoint{Vector2D(x, y), Vector2D(u, v)});
							}
							glm::mat4 transform(1.0f);
							transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
							mEngine->renderFan(navMeshTexture.getSurface(), drawablePolygon, transform, 0.3f);
						}
					}
				}
			}
		}
	}

	if (renderMode && renderMode->getIsDrawDebugAiPathsEnabled())
	{
		const Graphics::Texture& navMeshTexture = mResourceManager->getTexture(mNavmeshTextureHandle);
		world->getEntityManger().forEachComponentSet<AiControllerComponent>([drawShift, navMeshTexture, engine = mEngine](AiControllerComponent* aiController)
		{
			std::vector<Vector2D>& path = aiController->getPathRef().getSmoothPathRef();
			if (path.size() > 1)
			{
				std::vector<HAL::DrawPoint> drawablePolygon;
				drawablePolygon.reserve(path.size() * 2);

				{
					float u1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float u2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

					Vector2D normal = (path[1] - path[0]).normal() * 3;

					drawablePolygon.push_back(HAL::DrawPoint{path[0] + normal, Vector2D(u1, v1)});
					drawablePolygon.push_back(HAL::DrawPoint{path[0] - normal, Vector2D(u2, v2)});
				}

				for (size_t i = 1; i < path.size(); ++i)
				{
					float u1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float u2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

					Vector2D normal = (path[i] - path[i-1]).normal() * 3;

					drawablePolygon.push_back(HAL::DrawPoint{path[i] + normal, Vector2D(u1, v1)});
					drawablePolygon.push_back(HAL::DrawPoint{path[i] - normal, Vector2D(u2, v2)});
				}

				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
				engine->renderStrip(navMeshTexture.getSurface(), drawablePolygon, transform, 0.5f);
			}
		});
	}
}
