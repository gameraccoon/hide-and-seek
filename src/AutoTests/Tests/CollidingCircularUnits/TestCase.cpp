#include "AutoTests/Tests/CollidingCircularUnits/TestCase.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "GameData/Components/SpriteCreatorComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/StateMachineComponent.generated.h"

#include "GameLogic/Systems/RenderSystem.h"
#include "GameLogic/Systems/CollisionSystem.h"
#include "GameLogic/Systems/ResourceStreamingSystem.h"
#include "GameLogic/Systems/MovementSystem.h"
#include "GameLogic/Systems/CharacterStateSystem.h"

#include "GameLogic/ComponentsRegistration.h"

#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestCircularUnitsSystem.h"
#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

void CollidingCircularUnitsTestCase::start(const ArgumentsParser& arguments)
{
	getResourceManager()->loadAtlasesData("resources/atlas/atlas-list.json");

	mSystemsManager.registerSystem<TestUnitsCountControlSystem>(mWorldHolder);
	mSystemsManager.registerSystem<TestCircularUnitsSystem>(mWorldHolder);
	mSystemsManager.registerSystem<CharacterStateSystem>(mWorldHolder);
	mSystemsManager.registerSystem<MovementSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder);
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, getEngine(), getResourceManager());

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;
	mWorldHolder.gameData = &mGameData;

	Entity playerEntity = mWorld.getEntityManager().addEntity();
	{
		TransformComponent* transform = mWorld.getEntityManager().addComponent<TransformComponent>(playerEntity);
		transform->setLocation(Vector2D(0.0f, 0.0f));
	}
	{
		SpriteCreatorComponent* sprite = mWorld.getEntityManager().addComponent<SpriteCreatorComponent>(playerEntity);
		SpriteDescription spriteDesc;
		spriteDesc.params.size = Vector2D(30.0f, 30.0f);
		spriteDesc.path = "resources/textures/hero.png";
		sprite->getDescriptionsRef().emplace_back(std::move(spriteDesc));
	}
	{
		CollisionComponent* collision = mWorld.getEntityManager().addComponent<CollisionComponent>(playerEntity);
		Hull& hull = collision->getGeometryRef();
		hull.type = HullType::Circular;
		hull.setRadius(15.0f);
	}

	mWorld.setPlayerControlledEntity(playerEntity);
	mWorld.setMainCamera(playerEntity);

	mWorld.getWorldComponents().addComponent<StateMachineComponent>();

	mOneFrame = arguments.hasArgument("one-frame");

	// start the main loop
	getEngine()->start(this);
}

void CollidingCircularUnitsTestCase::update(float)
{
	constexpr float fixedDt = 0.16f;

	if (mOneFrame)
	{
		for (int i = mTicksCount; i < mTicksToFinish - 1; ++i)
		{
			mTime.update(fixedDt);
			mSystemsManager.update();
		}
		mTicksCount = mTicksToFinish - 1;
	}

	mTime.update(fixedDt);
	mSystemsManager.update();
	++mTicksCount;

	if (mTicksCount >= mTicksToFinish)
	{
		getEngine()->quit();
	}
}
