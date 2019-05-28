#include "StealthGame.h"

#include <memory>

#include "Base/Engine.h"

#include <Utils/WorldLoader.h>

#include <Systems/RenderSystem.h>
#include <Systems/ControlSystem.h>
#include <Systems/CollisionSystem.h>
#include <Systems/ResourceStreamingSystem.h>

#include <Components/TransformComponent.generated.h>
#include <Components/RenderComponent.generated.h>
#include <Components/CollisionComponent.generated.h>
#include <Components/MovementComponent.generated.h>
#include <Components/CameraComponent.generated.h>
#include <Components/LightComponent.generated.h>

namespace Game
{
	void StealthGame::start(ArgumentsParser& arguments)
	{
		mSystemsManager.registerSystem<ControlSystem>(getEngine(), &mKeyStates);
		mSystemsManager.registerSystem<CollisionSystem>();
		mSystemsManager.registerSystem<RenderSystem>(getEngine(), getResourceManager());
		mSystemsManager.registerSystem<ResourceStreamingSystem>(getResourceManager());

		mComponentFactory.registerComponent<TransformComponent>();
		mComponentFactory.registerComponent<RenderComponent>();
		mComponentFactory.registerComponent<CollisionComponent>();
		mComponentFactory.registerComponent<MovementComponent>();
		mComponentFactory.registerComponent<CameraComponent>();
		mComponentFactory.registerComponent<LightComponent>();

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

	void StealthGame::setKeyState(int key, bool isPressed)
	{
		mKeyStates[key] = isPressed;
	}

	void StealthGame::update(float dt)
	{
		mSystemsManager.update(&mWorld, dt);
	}
}
