#pragma once

#include "IGame.h"

#include "Base/ResourceManager.h"
#include <EngineFwd.h>

namespace SystemInterface
{
	class Game : public IGame
	{
	protected:
		Game();
		virtual ~Game();

		Engine* getEngine() { return mEngine.get(); }
		ResourceManager* getResourceManager() { return mResourceManager.get(); }

	private:
		// order is valuable: engine and then resource manager 
		std::unique_ptr<Engine> mEngine;
		std::unique_ptr<ResourceManager> mResourceManager;
	};
}
