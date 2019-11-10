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
#include "GameLogic/Systems/AnimationSystem.h"

#include "GameLogic/ComponentsRegistration.h"
#include "GameLogic/Initialization/StateMachines.h"

void Game::start(ArgumentsParser& arguments)
{
	initSystems();

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	GameDataLoader::LoadWorld(mWorld, arguments.getArgumentValue("world", "test"), mComponentFactory);
	GameDataLoader::LoadGameData(mGameData, arguments.getArgumentValue("gameData", "gameData"), mComponentFactory);

	// ToDo: make an editor not to hardcode SM data
	auto [sm] = mGameData.getGameComponents().getComponents<StateMachineComponent>();
	StateMachines::RegisterStateMachines(sm);

	mProfileSystems = arguments.hasArgument("profile-systems");
	mSystemProfileOutputPath = arguments.getArgumentValue("profile-systems", mSystemProfileOutputPath);

	// start the main loop
	getEngine().start(this);

	onGameShutdown();
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

	if (mProfileSystems)
	{
		mSystemFrameRecords.addFrame(mSystemsManager.getPreviousFrameTimeData());
	}
}

void Game::initSystems()
{
	mSystemsManager.registerSystem<ControlSystem>(mWorldHolder, getEngine(), mKeyStates);
	mSystemsManager.registerSystem<AiSystem>(mWorldHolder);
	mSystemsManager.registerSystem<CharacterStateSystem>(mWorldHolder);
	mSystemsManager.registerSystem<MovementSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder);
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<AnimationSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, getEngine(), getResourceManager(), mJobsWorkerManager);
	mSystemsManager.registerSystem<DebugDrawSystem>(mWorldHolder, mTime, getEngine(), getResourceManager());
}

void Game::initResources()
{
	getResourceManager().loadAtlasesData("resources/atlas/atlas-list.json");
	mSystemsManager.initResources();
}

void Game::onGameShutdown()
{
	if (mProfileSystems)
	{
		mSystemFrameRecords.printToFile(mSystemsManager.getSystemNames(), mSystemProfileOutputPath);
	}
}
