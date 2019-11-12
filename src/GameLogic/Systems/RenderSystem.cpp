#include "GameLogic/Systems/RenderSystem.h"

#include <algorithm>

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/VisibilityPolygon.h"

#include "HAL/Base/Math.h"
#include "HAL/Base/Engine.h"
#include "HAL/Graphics/Sprite.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::RenderSystem(WorldHolder& worldHolder, HAL::Engine& engine, HAL::ResourceManager& resourceManager, Jobs::WorkerManager& jobsWorkerManager)
	: mWorldHolder(worldHolder)
	, mEngine(engine)
	, mResourceManager(resourceManager)
	, mJobsWorkerManager(jobsWorkerManager)
{
	mLightSpriteHandle = resourceManager.lockSprite("resources/textures/light.png");
}

void RenderSystem::update()
{
	World& world = mWorldHolder.getWorld();
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

	auto [renderMode] = world.getWorldComponents().getComponents<RenderModeComponent>();

	Vector2D cameraLocation = cameraTransformComponent->getLocation();
	Vector2D mouseScreenPos(mEngine.getMouseX(), mEngine.getMouseY());
	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine.getWidth()), static_cast<float>(mEngine.getHeight())) * 0.5f;

	Vector2D drawShift = screenHalfSize - cameraLocation + (screenHalfSize - mouseScreenPos) * 0.5;

	if (!renderMode || renderMode->getIsDrawLightsEnabled())
	{
		drawLights(world, drawShift, maxFov, screenHalfSize);
	}

	if (!renderMode || renderMode->getIsDrawVisibleEntitiesEnabled())
	{
		world.getEntityManager().forEachComponentSet<RenderComponent, TransformComponent>([&drawShift, &resourceManager = mResourceManager, &renderer](RenderComponent* render, TransformComponent* transform)
		{
			auto location = transform->getLocation() + drawShift;
			float rotation = transform->getRotation().getValue();
			for (const auto& data : render->getSpriteDatas())
			{
				const Graphics::Sprite& spriteData = resourceManager.getResource<Graphics::Sprite>(data.spriteHandle);
				renderer.render(*spriteData.getTexture(), location, data.params.size, data.params.anchor, rotation, spriteData.getUV(), 1.0f);
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
		mEngine.getRenderer().renderFan(*lightSprite.getTexture(), drawablePolygon, transform, 0.5f);
	}
}

Vector2D RenderSystem::GetPlayerSightPosition(World& world)
{
	Vector2D result(0.0f, 0.0f);

	if (OptionalEntity playerEntity = world.getPlayerControlledEntity(); playerEntity.isValid())
	{
		auto [playerTransform] = world.getEntityManager().getEntityComponents<TransformComponent>(playerEntity.getEntity());

		if (playerTransform != nullptr)
		{
			result = playerTransform->getLocation();
		}
	}

	return result;
}

class VisibilityPolygonCalculationJob : public Jobs::BaseJob
{
public:
	struct Result
	{
		std::vector<Vector2D> polygon;
		Vector2D location;
	};

	using FinalizeFn = std::function<void(const std::vector<Result>&, Vector2D)>;
	using CollidableComponents = std::vector<std::tuple<CollisionComponent*, TransformComponent*>>;

public:
	VisibilityPolygonCalculationJob(Vector2D maxFov, const CollidableComponents& collidableComponents, FinalizeFn finalizeFn)
		: mMaxFov(maxFov)
		, mCollidableComponents(collidableComponents)
		, mFinalizeFn(std::move(finalizeFn))
	{
		Assert(mFinalizeFn, "finalizeFn should be set");
	}

	void process() override
	{
		VisibilityPolygonCalculator visibilityPolygonCalculator;
		mCalculationResults.reserve(componentsToProcess.size());
		TransformComponent* transform;
		for (auto components : componentsToProcess)
		{
			std::tie(std::ignore, transform) = components;
			mCalculationResults.emplace_back();
			visibilityPolygonCalculator.calculateVisibilityPolygon(mCalculationResults.back().polygon, mCollidableComponents, transform->getLocation(), mMaxFov);
			mCalculationResults.back().location = transform->getLocation();
		}
	}

	void finalize() override
	{
		if (mFinalizeFn)
		{
			mFinalizeFn(mCalculationResults, mMaxFov);
		}
	}

public:
	std::vector<std::tuple<LightComponent*, TransformComponent*>> componentsToProcess;

private:
	Vector2D mMaxFov;
	const CollidableComponents& mCollidableComponents;
	FinalizeFn mFinalizeFn;

	std::vector<Result> mCalculationResults;
};

static size_t GetJobDivisor(size_t maxThreadsCount)
{
	// this alghorithm is subject to change
	// we need to divide work into chunks to pass to different threads
	// take to consideration that the count of free threads most likely
	// smaller that threadsCount and can fluctuate over time
	return maxThreadsCount * 3 - 1;
}

void RenderSystem::drawLights(World& world, const Vector2D& drawShift, const Vector2D& maxFov, const Vector2D& screenHalfSize)
{
	const Graphics::Sprite& lightSprite = mResourceManager.getResource<Graphics::Sprite>(mLightSpriteHandle);
	if (!lightSprite.isValid())
	{
		return;
	}

	Vector2D playerSightPosition = GetPlayerSightPosition(world);

	const auto collidableComponents = world.getEntityManager().getComponents<CollisionComponent, TransformComponent>();

	std::vector<std::tuple<LightComponent*, TransformComponent*>> componentSets = world.getEntityManager().getComponents<LightComponent, TransformComponent>();

	Vector2D emitterPositionBordersLT = playerSightPosition - screenHalfSize - maxFov*0.5;
	Vector2D emitterPositionBordersRB = playerSightPosition + screenHalfSize + maxFov*0.5;

	componentSets.erase(
		std::remove_if(
			std::begin(componentSets),
			std::end(componentSets),
			[emitterPositionBordersLT, emitterPositionBordersRB](auto& componentSet)
			{
				return !std::get<1>(componentSet)->getLocation().isInside(emitterPositionBordersLT, emitterPositionBordersRB);
			}
		),
		std::end(componentSets)
	);

	if (!componentSets.empty())
	{
		size_t threadsCount = mJobsWorkerManager.getThreadsCount();
		AssertFatal(threadsCount != 0, "Jobs Worker Manager threads count can't be zero");

		size_t chunksCount = GetJobDivisor(threadsCount + 1);
		size_t chunkSize = std::max((componentSets.size() / chunksCount) + (componentSets.size() % chunksCount > 0 ? 1 : 0), 1ul);

		std::vector<Jobs::BaseJob::UniquePtr> jobs;
		size_t chunkItemIndex = 0;

		VisibilityPolygonCalculationJob::FinalizeFn finalizeFn = [this, drawShift, &lightSprite](const std::vector<VisibilityPolygonCalculationJob::Result>& results, Vector2D maxFov)
		{
			for (auto& calcResult : results)
			{
				this->drawVisibilityPolygon(lightSprite, calcResult.polygon, maxFov, drawShift + calcResult.location);
			}
		};

		for (auto& set : componentSets)
		{
			if (chunkItemIndex == 0)
			{
				jobs.emplace_back(new VisibilityPolygonCalculationJob(maxFov, collidableComponents, finalizeFn));
			}

			VisibilityPolygonCalculationJob* jobData = static_cast<VisibilityPolygonCalculationJob*>(jobs.rbegin()->get());

			jobData->componentsToProcess.emplace_back(set);

			++chunkItemIndex;
			if (chunkItemIndex >= chunkSize)
			{
				chunkItemIndex = 0;
			}
		}

		mJobsWorkerManager.runJobs(std::move(jobs));

		VisibilityPolygonCalculator visibilityPolygonCalculator;

		std::vector<Vector2D> polygon;
		// draw player visibility polygon
		visibilityPolygonCalculator.calculateVisibilityPolygon(polygon, collidableComponents, playerSightPosition, maxFov);
		drawVisibilityPolygon(lightSprite, polygon, maxFov, drawShift + playerSightPosition);
	}
}
