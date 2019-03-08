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
		std::shared_ptr<ResourceManager> getResourceManager() { return mResourceManager; }

	private:
		std::unique_ptr<Engine> mEngine;
		std::shared_ptr<ResourceManager> mResourceManager;
	};
}
