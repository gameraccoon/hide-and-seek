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

#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestCircularUnitsSystem.h"
#include "AutoTests/Tests/CollidingCircularUnits/Systems/TestUnitsCountControlSystem.h"

void CollidingCircularUnitsTestCase::initTestCase(const ArgumentsParser& /*arguments*/)
{
	getResourceManager().loadAtlasesData("resources/atlas/atlas-list.json");

	mSystemsManager.registerSystem<TestUnitsCountControlSystem>(mWorldHolder);
	mSystemsManager.registerSystem<TestCircularUnitsSystem>(mWorldHolder);
	mSystemsManager.registerSystem<CharacterStateSystem>(mWorldHolder);
	mSystemsManager.registerSystem<MovementSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, mTime, getEngine(), getResourceManager(), mWorkerManager);

	EntityView playerEntity = mWorld.createTrackedSpatialEntity(STR_TO_ID("ControlledEntity"), CellPos(0, 0));
	{
		TransformComponent* transform = playerEntity.addComponent<TransformComponent>();
		transform->setLocation(Vector2D(0.0f, 0.0f));
	}
	{
		SpriteCreatorComponent* sprite = playerEntity.addComponent<SpriteCreatorComponent>();
		SpriteDescription spriteDesc;
		spriteDesc.params.size = Vector2D(30.0f, 30.0f);
		spriteDesc.path = "resources/textures/hero.png";
		sprite->getDescriptionsRef().emplace_back(std::move(spriteDesc));
	}
	{
		CollisionComponent* collision = playerEntity.addComponent<CollisionComponent>();
		Hull& hull = collision->getGeometryRef();
		hull.type = HullType::Circular;
		hull.setRadius(15.0f);
	}

	mWorld.createTrackedSpatialEntity(STR_TO_ID("CameraEntity"), CellPos(0, 0));

	mWorld.getWorldComponents().addComponent<StateMachineComponent>();
}
