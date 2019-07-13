#include "GameLogic/Game.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "Utils/World/WorldLoader.h"

#include "GameLogic/Systems/RenderSystem.h"
#include "GameLogic/Systems/ControlSystem.h"
#include "GameLogic/Systems/CollisionSystem.h"
#include "GameLogic/Systems/ResourceStreamingSystem.h"
#include "GameLogic/Systems/AiSystem.h"
#include "GameLogic/Systems/DebugDrawSystem.h"

#include "GameLogic/ComponentsRegistration.h"

void Game::start(ArgumentsParser& arguments)
{
	initResources();
	initSystems();

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;

	if (arguments.hasArgument("world"))
	{
		WorldLoader::LoadWorld(mWorld, arguments.getArgumentValue("world"), mComponentFactory);
	}
	else
	{
		WorldLoader::LoadWorld(mWorld, "test", mComponentFactory);
	}

	// start the main loop
	getEngine()->start(this);
}

void Game::setKeyState(int key, bool isPressed)
{
	mKeyStates.updateState(key, isPressed);
}

void Game::update(float dt)
{
	mTime.update(dt);
	mSystemsManager.update();
	mKeyStates.clearLastFrameState();
}

void Game::initSystems()
{
	mSystemsManager.registerSystem<ControlSystem>(mWorldHolder, mTime, getEngine(), &mKeyStates);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder);
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, getEngine(), getResourceManager());
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<AiSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<DebugDrawSystem>(mWorldHolder, getEngine(), getResourceManager());
}

void Game::initResources()
{
	getResourceManager()->loadAtlasesData("resources/atlas/atlas-list.json");
}
