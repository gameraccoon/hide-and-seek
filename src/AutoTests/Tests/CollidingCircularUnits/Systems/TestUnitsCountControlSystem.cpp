#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/SpriteCreatorComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"

#include "GameData/World.h"
#include "GameData/Spatial/SpatialWorldData.h"

static const int jitterRand = 500;
static const float jitterMax = 30.0f;
static const float halfJitterMax = jitterMax / 2.0f;
static const float jitterDivider = jitterRand / jitterMax;
static const float distance = 50.0f;

TestUnitsCountControlSystem::TestUnitsCountControlSystem(WorldHolder& worldHolder)
	: mWorldHolder(worldHolder)
{
}

static void spawnUnit(EntityManager& entityManager, Vector2D pos)
{
	Entity entity = entityManager.addEntity();
	{
		TransformComponent* transform = entityManager.addComponent<TransformComponent>(entity);
		transform->setLocation(pos);
	}
	{
		MovementComponent* movement = entityManager.addComponent<MovementComponent>(entity);
		movement->setOriginalSpeed(1.0f);
	}
	{
		SpriteCreatorComponent* sprite = entityManager.addComponent<SpriteCreatorComponent>(entity);
		SpriteDescription spriteDesc;
		spriteDesc.params.size = Vector2D(20.0f, 20.0f);
		spriteDesc.path = "resources/textures/hero.png";
		sprite->getDescriptionsRef().emplace_back(std::move(spriteDesc));
	}
	{
		CollisionComponent* collision = entityManager.addComponent<CollisionComponent>(entity);
		Hull& hull = collision->getGeometryRef();
		hull.type = HullType::Circular;
		hull.setRadius(10.0f);
	}
	entityManager.addComponent<AiControllerComponent>(entity);
	entityManager.addComponent<CharacterStateComponent>(entity);
}

static void spawnJitteredUnit(const Vector2D& pos, const Vector2D& centerShifted, SpatialWorldData& spatialData)
{
	Vector2D jitter = Vector2D((rand() % jitterRand) / jitterDivider - halfJitterMax, ((rand() % jitterRand) / jitterDivider - halfJitterMax));
	Vector2D newPos = centerShifted + pos + jitter;
	std::pair<CellPos, Vector2D> transformedPos = SpatialWorldData::GetTransformedCellPos(CellPos(0, 0), newPos);
	spawnUnit(spatialData.getOrCreateCell(transformedPos.first).getEntityManager(), transformedPos.second);
}

static void spawnUnits(SpatialWorldData& spatialData, int count, Vector2D pos)
{
	int n = static_cast<int>(std::sqrt(count));
	int m = count / n;

	Vector2D centerShifted = pos - Vector2D(static_cast<float>(n - 1), m - ((count == m * n) ? 1.0f : 0.0f)) * (distance * 0.5f);

	for (int j = 0; j < m; ++j)
	{
		for (int i = 0; i < n; ++i)
		{
			spawnJitteredUnit(Vector2D(i * distance, j * distance), centerShifted, spatialData);
		}
	}

	float yPos = m * distance;
	int unitsLeft = count - m * n;
	for (int i = 0; i < unitsLeft; ++i)
	{
		spawnJitteredUnit(Vector2D(i * distance, yPos), centerShifted, spatialData);
	}
}

void TestUnitsCountControlSystem::update()
{
	World& world = mWorldHolder.getWorld();

	if (ticksPassed == 5)
	{
		spawnUnits(world.getSpatialData(), 500, Vector2D(-400.0f, 0.0f));
	}

	++ticksPassed;
}
