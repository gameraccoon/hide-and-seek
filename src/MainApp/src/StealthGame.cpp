#define GLM_FORCE_RADIANS

#include <memory>

#include "StealthGame.h"

#include "Base/Engine.h"

#include <Modules/ActorFactory.h>

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();
		mMainCamera = std::make_unique<Camera>(mWorld.get(),
            Vector2D(static_cast<float>(getEngine()->getWidth()), static_cast<float>(getEngine()->getHeight())),
			Vector2D(0.0f, 0.0f));

		ActorFactory::Factory().spawnActor("Man", mWorld.get(), Vector2D(10, 10),
			Vector2D(1, 1), Rotator(0));

		// start the main loop
		getEngine()->start(this);
	}

	void StealthGame::update(float dt)
	{
		mWorld->update(dt);
	}

	void StealthGame::draw()
	{
		mMainCamera->render();
	}
}
