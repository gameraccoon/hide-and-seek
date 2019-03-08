#define GLM_FORCE_RADIANS

#include <memory>

#include "StealthGame.h"

#include "Base/Engine.h"

#include <Modules/ActorFactory.h>

#include <Systems/RenderSystem.h>
#include <Systems/ControlSystem.h>

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();

		mSystemsManager = std::make_unique<SystemsManager>();
		mSystemsManager->registerSystem<ControlSystem>(getEngine(), &mKeyStates);
		mSystemsManager->registerSystem<RenderSystem>(getResourceManager());

		IActor* hero = ActorFactory::Factory().spawnActor("Hero", mWorld.get(), Vector2D(10, 10),
			Vector2D(1, 1), Rotator(0));

		if (auto transformComponent = hero->getSingleComponent<TransformComponent>())
		{
			mWorld->setPlayerControlledTransform(transformComponent);
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
		mWorld->update(dt);
		mSystemsManager->update(mWorld.get(), dt);
	}
}
