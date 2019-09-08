#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/SpriteCreatorComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"

#include "GameData/World.h"


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
		movement->setOriginalSpeed(30.0f);
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

static void spawnUnits(EntityManager& entityManager, int count, Vector2D pos)
{
	int n = static_cast<int>(std::sqrt(count));
	int m = count / n;
	constexpr float distance = 50.0f;

	Vector2D centerShifted = pos - Vector2D(n - 1, m - ((count == m * n) ? 1 : 0)) * (distance / 2.0f);

	constexpr int jitterRand = 500;
	constexpr float jitterMax = 30.0f;
	constexpr float halfJitterMax = jitterMax / 2.0f;
	constexpr float jitterDivider = jitterRand / jitterMax;

	for (int j = 0; j < m; ++j)
	{
		for (int i = 0; i < n; ++i)
		{
			Vector2D jitter = Vector2D((rand() % jitterRand) / jitterDivider - halfJitterMax, ((rand() % jitterRand) / jitterDivider - halfJitterMax));
			spawnUnit(entityManager, centerShifted + Vector2D(i * distance, j * distance) + jitter);
		}
	}

	float yPos = m * distance;
	int unitsLeft = count - m * n;
	for (int i = 0; i < unitsLeft; ++i)
	{
		spawnUnit(entityManager, centerShifted + Vector2D(i * distance, yPos));
	}
}

void TestUnitsCountControlSystem::update()
{
	World* world = mWorldHolder.world;

	if (ticksPassed == 5)
	{
		spawnUnits(world->getEntityManager(), 500, Vector2D(-400.0f, 0.0f));
	}

	++ticksPassed;
}
