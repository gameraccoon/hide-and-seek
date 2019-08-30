#include "GameLogic/Game.h"

#include <memory>

#include "GameData/Components/StateMachineComponent.generated.h"

#include "HAL/Base/Engine.h"

#include "Utils/World/GameDataLoader.h"

#include "GameLogic/Systems/RenderSystem.h"
#include "GameLogic/Systems/ControlSystem.h"
#include "GameLogic/Systems/CollisionSystem.h"
#include "GameLogic/Systems/ResourceStreamingSystem.h"
#include "GameLogic/Systems/AiSystem.h"
#include "GameLogic/Systems/CharacterStateSystem.h"
#include "GameLogic/Systems/DebugDrawSystem.h"
#include "GameLogic/Systems/MovementSystem.h"

#include "GameLogic/ComponentsRegistration.h"
#include "GameLogic/Initialization/StateMachines.h"

void Game::start(ArgumentsParser& arguments)
{
	initResources();
	initSystems();

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;
	mWorldHolder.gameData = &mGameData;

	GameDataLoader::LoadWorld(mWorld, arguments.getArgumentValue("world", "test"), mComponentFactory);
	GameDataLoader::LoadGameData(mGameData, arguments.getArgumentValue("gameData", "gameData"), mComponentFactory);

	// ToDo: make an editor not to hardcode SM data
	auto [sm] = mGameData.getGameComponents().getComponents<StateMachineComponent>();
	StateMachines::RegisterStateMachines(sm);

	// start the main loop
	getEngine()->start(this);
}

void Game::setKeyboardKeyState(int key, bool isPressed)
{
	mKeyStates.updateState(key, isPressed);
}

void Game::setMouseKeyState(int /*key*/, bool /*isPressed*/)
{
}

void Game::update(float dt)
{
	mTime.update(dt);
	mSystemsManager.update();
	mKeyStates.clearLastFrameState();
}

void Game::initSystems()
{
	mSystemsManager.registerSystem<ControlSystem>(mWorldHolder, getEngine(), &mKeyStates);
	mSystemsManager.registerSystem<AiSystem>(mWorldHolder);
	mSystemsManager.registerSystem<CharacterStateSystem>(mWorldHolder);
	mSystemsManager.registerSystem<MovementSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder);
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, getEngine(), getResourceManager());
	mSystemsManager.registerSystem<DebugDrawSystem>(mWorldHolder, getEngine(), getResourceManager());
}

void Game::initResources()
{
	getResourceManager()->loadAtlasesData("resources/atlas/atlas-list.json");
}
