#include "AutoTests/Tests/CollidingCircularUnits/TestCase.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "Utils/World/WorldLoader.h"

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

#include "GameLogic/Systems/RenderSystem.h"
#include "GameLogic/Systems/CollisionSystem.h"
#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameLogic/ComponentsRegistration.h"

#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestCircularUnitsSystem.h"
#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

void CollidingCircularUnitsTestCase::start(ArgumentsParser& /*arguments*/)
{
	mSystemsManager.registerSystem<TestUnitsCountControlSystem>();
	mSystemsManager.registerSystem<TestCircularUnitsSystem>();
	mSystemsManager.registerSystem<CollisionSystem>();
	mSystemsManager.registerSystem<RenderSystem>(getEngine(), getResourceManager());
	mSystemsManager.registerSystem<ResourceStreamingSystem>(getResourceManager());

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	Entity playerEntity = mWorld.getEntityManger().addEntity();
	TransformComponent* transform = mWorld.getEntityManger().addComponent<TransformComponent>(playerEntity);
	transform->setLocation(Vector2D(0.0f, 0.0f));
	RenderComponent* render = mWorld.getEntityManger().addComponent<RenderComponent>(playerEntity);
	render->setSize(Vector2D(30.0f, 30.0f));
	render->setTexturePath("resources/textures/hero.png");
	CollisionComponent* collision = mWorld.getEntityManger().addComponent<CollisionComponent>(playerEntity);
	Hull& hull = collision->getGeometryRef();
	hull.type = Hull::Type::Circular;
	hull.setRadius(15.0f);

	mWorld.setPlayerControlledEntity(playerEntity);
	mWorld.setMainCamera(playerEntity);

	// start the main loop
	getEngine()->start(this);
}

void CollidingCircularUnitsTestCase::update(float)
{
	if (ticksCount == 100)
	{
		getEngine()->quit();
	}

	constexpr float fixedDt = 0.16f;
	mSystemsManager.update(&mWorld, fixedDt);
	++ticksCount;
}
