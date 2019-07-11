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
	mSystemsManager.update(&mWorld, dt);
	mKeyStates.clearLastFrameState();
}

void Game::initSystems()
{
	mSystemsManager.registerSystem<ControlSystem>(getEngine(), &mKeyStates);
	mSystemsManager.registerSystem<CollisionSystem>();
	mSystemsManager.registerSystem<RenderSystem>(getEngine(), getResourceManager());
	mSystemsManager.registerSystem<ResourceStreamingSystem>(getResourceManager());
	mSystemsManager.registerSystem<AiSystem>();
	mSystemsManager.registerSystem<DebugDrawSystem>(getEngine(), getResourceManager());
}

void Game::initResources()
{
	getResourceManager()->loadAtlasesData("resources/atlas/atlas-list.json");
}
