#include "GameLogic/Systems/DebugDrawSystem.h"

#include <algorithm>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Geometry/VisibilityPolygon.h"

#include "HAL/Base/Engine.h"
#include "HAL/Base/Math.h"
#include "HAL/Graphics/Renderer.h"
#include "HAL/Graphics/Sprite.h"
#include "HAL/Graphics/Font.h"

#include <glm/gtc/matrix_transform.hpp>

#include <DetourNavMesh.h>


DebugDrawSystem::DebugDrawSystem(WorldHolder& worldHolder, const TimeData& timeData, HAL::Engine& engine, HAL::ResourceManager& resourceManager)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
	, mEngine(engine)
	, mResourceManager(resourceManager)
{
}

void DebugDrawSystem::update()
{
	World& world = mWorldHolder.getWorld();
	GameData& gameData = mWorldHolder.getGameData();
	Graphics::Renderer& renderer = mEngine.getRenderer();

	static const Vector2D maxFov(500.0f, 500.0f);

	OptionalEntity mainCamera = world.getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	auto [cameraTransformComponent] = world.getEntityManager().getEntityComponents<TransformComponent>(mainCamera.getEntity());
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D mouseScreenPos(mEngine.getMouseX(), mEngine.getMouseY());
	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine.getWidth()), static_cast<float>(mEngine.getHeight())) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation + (screenHalfSize - mouseScreenPos) * 0.5;

	auto [renderMode] = gameData.getGameComponents().getComponents<RenderModeComponent>();
	if (renderMode && renderMode->getIsDrawDebugCollisionsEnabled())
	{
		const Graphics::Sprite& collisionSprite = mResourceManager.getResource<Graphics::Sprite>(mCollisionSpriteHandle);
		Graphics::QuadUV quadUV = collisionSprite.getUV();
		world.getEntityManager().forEachComponentSet<CollisionComponent>([&collisionSprite, &quadUV, drawShift, &renderer](CollisionComponent* collisionComponent)
		{
			renderer.render(*collisionSprite.getTexture(),
				Vector2D(collisionComponent->getBoundingBox().minX + drawShift.x, collisionComponent->getBoundingBox().minY + drawShift.y),
				Vector2D(collisionComponent->getBoundingBox().maxX-collisionComponent->getBoundingBox().minX,
						 collisionComponent->getBoundingBox().maxY-collisionComponent->getBoundingBox().minY),
				ZERO_VECTOR,
				0.0f,
				quadUV);
			return true;
		});
	}

	if (renderMode && renderMode->getIsDrawDebugAiDataEnabled())
	{
		const Graphics::Sprite& navMeshSprite = mResourceManager.getResource<Graphics::Sprite>(mNavmeshSpriteHandle);
		Graphics::QuadUV quadUV = navMeshSprite.getUV();
		auto [navMeshComponent] = world.getWorldComponents().getComponents<NavMeshComponent>();

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
						for (int k = 0; k < tile->header->polyCount; ++k)
						{
							const auto& poly = tile->polys[k];
							std::vector<Graphics::DrawPoint> drawablePolygon;
							drawablePolygon.reserve(3);
							for (int j = 0; j < poly.vertCount; ++j)
							{
								float u = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
								float v = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

								float x = tile->verts[poly.verts[j] * 3];
								float y = tile->verts[poly.verts[j] * 3 + 2];
								drawablePolygon.push_back(Graphics::DrawPoint{Vector2D(x, y), Graphics::QuadLerp(quadUV, u, v)});
							}
							glm::mat4 transform(1.0f);
							transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
							renderer.renderFan(*navMeshSprite.getTexture(), drawablePolygon, transform, 0.3f);
						}
					}
				}
			}
		}

		world.getEntityManager().forEachComponentSet<AiControllerComponent>([drawShift, &quadUV, &navMeshSprite, &renderer](AiControllerComponent* aiController)
		{
			std::vector<Vector2D>& path = aiController->getPathRef().getSmoothPathRef();
			if (path.size() > 1)
			{
				std::vector<Graphics::DrawPoint> drawablePolygon;
				drawablePolygon.reserve(path.size() * 2);

				{
					float u1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float u2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

					Vector2D normal = (path[1] - path[0]).normal() * 3;

					drawablePolygon.push_back(Graphics::DrawPoint{path[0] + normal, Graphics::QuadLerp(quadUV, u1, v1)});
					drawablePolygon.push_back(Graphics::DrawPoint{path[0] - normal, Graphics::QuadLerp(quadUV, u2, v2)});
				}

				for (size_t i = 1; i < path.size(); ++i)
				{
					float u1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v1 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float u2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);
					float v2 = static_cast<float>(std::rand() * 1.0f / RAND_MAX);

					Vector2D normal = (path[i] - path[i-1]).normal() * 3;

					drawablePolygon.push_back(Graphics::DrawPoint{path[i] + normal, Graphics::QuadLerp(quadUV, u1, v1)});
					drawablePolygon.push_back(Graphics::DrawPoint{path[i] - normal, Graphics::QuadLerp(quadUV, u2, v2)});
				}

				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, glm::vec3(drawShift.x, drawShift.y, 0.0f));
				renderer.renderStrip(*navMeshSprite.getTexture(), drawablePolygon, transform, 0.5f);
			}
		});
	}

	if (renderMode && renderMode->getIsDrawDebugFpsEnabled())
	{
		const Graphics::Font& font = mResourceManager.getResource<Graphics::Font>(mFontHandle);
		float dt = mTime.dt;
		static Graphics::Color normalColor{255, 255, 255, 255};
		static Graphics::Color badColor{255, 145, 0, 255};
		static Graphics::Color veryBadColor{255, 0, 0, 255};

		int fps = static_cast<int>(1/dt);

		Graphics::Color fpsColor;
		if (fps >= 30)
		{
			fpsColor = normalColor;
		}
		else if (fps > 20)
		{
			fpsColor = badColor;
		}
		else
		{
			fpsColor = veryBadColor;
		}

		renderer.renderText(font, Vector2D(20.0f, 20.0f), fpsColor, std::string("FPS: ").append(std::to_string(fps)).c_str());
		renderer.renderText(font, Vector2D(20.0f, 40.0f), {255, 255, 255, 255}, std::string("ms: ").append(std::to_string(static_cast<int>(dt*1000))).c_str());
	}

	if (renderMode && renderMode->getIsDrawDebugCharacterInfoEnabled())
	{
		const Graphics::Font& font = mResourceManager.getResource<Graphics::Font>(mFontHandle);
		world.getEntityManager().forEachComponentSet<CharacterStateComponent, TransformComponent>([&renderer, &font, drawShift](CharacterStateComponent* characterState, TransformComponent* transform)
		{
			renderer.renderText(font, transform->getLocation() + drawShift, {255, 255, 255, 255}, enum_to_string(characterState->getState()).c_str());
		});
	}
}

void DebugDrawSystem::initResources()
{
	mCollisionSpriteHandle = mResourceManager.lockSprite("resources/textures/collision.png");
	mNavmeshSpriteHandle = mResourceManager.lockSprite("resources/textures/testTexture.png");
	mFontHandle = mResourceManager.lockFont("resources/fonts/prstart.ttf", 16);
}
