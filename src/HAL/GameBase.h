#pragma once

#include "IGame.h"

#include "Base/ResourceManager.h"
#include "HAL/EngineFwd.h"

namespace HAL
{
	class GameBase : public IGame
	{
	public:
		GameBase(int windowWidth, int windowHeight);
		~GameBase();

	protected:
		Engine* getEngine() { return mEngine.get(); }
		ResourceManager* getResourceManager() { return mResourceManager.get(); }

	private:
		std::unique_ptr<Engine> mEngine;
		std::unique_ptr<ResourceManager> mResourceManager;
	};
}
