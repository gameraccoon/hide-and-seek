#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"

#include "GameData/World.h"


TestUnitsCountControlSystem::TestUnitsCountControlSystem(WorldHolder& worldHolder)
	: mWorldHolder(worldHolder)
{
}

static void spawnUnit(EntityManager& entityManager, Vector2D pos)
{
	Entity entity = entityManager.addEntity();
	TransformComponent* transform = entityManager.addComponent<TransformComponent>(entity);
	transform->setLocation(pos);
	entityManager.addComponent<MovementComponent>(entity);
	RenderComponent* render = entityManager.addComponent<RenderComponent>(entity);
	render->setSize(Vector2D(20.0f, 20.0f));
	render->setTexturePath("resources/textures/hero.png");
	CollisionComponent* collision = entityManager.addComponent<CollisionComponent>(entity);
	Hull& hull = collision->getGeometryRef();
	hull.type = Hull::Type::Circular;
	hull.setRadius(10.0f);
	entityManager.addComponent<AiControllerComponent>(entity);
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
		spawnUnits(world->getEntityManger(), 500, Vector2D(-400.0f, 0.0f));
	}

	++ticksPassed;
}
