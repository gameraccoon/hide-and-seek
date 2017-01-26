#define GLM_FORCE_RADIANS

#include "StealthGame.h"

#include "Base/Engine.h"

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();
		mMainCamera = std::make_unique<Camera>(mWorld.get(),
			Vector2D((float)getEngine()->getWidth(), (float)getEngine()->getHeight()),
			Vector2D(0.0f, 0.0f));

		// start the main loop
		getEngine()->start(*this);
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
