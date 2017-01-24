#define GLM_FORCE_RADIANS

#include "StealthGame.h"

#include "Base/Engine.h"

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();

		// start the main loop
		getEngine()->start(*this);
	}

	void StealthGame::update(float dt)
	{
		mWorld->update(dt);
	}

	void StealthGame::draw()
	{
		
	}
}
