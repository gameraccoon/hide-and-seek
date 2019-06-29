#pragma once

#include "IGame.h"

#include "Base/ResourceManager.h"
#include "HAL/EngineFwd.h"

namespace HAL
{
	class GameBase : public IGame
	{
	protected:
		GameBase();
		virtual ~GameBase();

		Engine* getEngine() { return mEngine.get(); }
		std::shared_ptr<ResourceManager> getResourceManager() { return mResourceManager; }

	private:
		std::unique_ptr<Engine> mEngine;
		std::shared_ptr<ResourceManager> mResourceManager;
	};
}
