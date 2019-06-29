#include "Game/Game.h"

#include <memory>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CameraComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/NavMeshComponent.generated.h"

#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/ControlSystem.h"
#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/ResourceStreamingSystem.h"
#include "Game/Systems/AiSystem.h"
#include "Game/Systems/DebugDrawSystem.h"

#include "HAL/Base/Engine.h"

#include "Utils/World/WorldLoader.h"

void Game::start(ArgumentsParser& arguments)
{
	mSystemsManager.registerSystem<ControlSystem>(getEngine(), &mKeyStates);
	mSystemsManager.registerSystem<CollisionSystem>();
	mSystemsManager.registerSystem<RenderSystem>(getEngine(), getResourceManager());
	mSystemsManager.registerSystem<ResourceStreamingSystem>(getResourceManager());
	mSystemsManager.registerSystem<AiSystem>();
	mSystemsManager.registerSystem<DebugDrawSystem>(getEngine(), getResourceManager());

	mComponentFactory.registerComponent<TransformComponent>();
	mComponentFactory.registerComponent<RenderComponent>();
	mComponentFactory.registerComponent<CollisionComponent>();
	mComponentFactory.registerComponent<MovementComponent>();
	mComponentFactory.registerComponent<CameraComponent>();
	mComponentFactory.registerComponent<LightComponent>();
	mComponentFactory.registerComponent<RenderModeComponent>();
	mComponentFactory.registerComponent<NavMeshComponent>();
	mComponentFactory.registerComponent<AiControllerComponent>();

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
