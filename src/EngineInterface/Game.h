#pragma once

#include "IGame.h"

#include "Internal/Engine.h"
#include "Graphics/ResourceManager.h"

namespace Engine
{
	class Game : public IGame
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void Draw() = 0;

	protected:
		Game();
		~Game() = default;

		Engine::Internal::Engine* getEngine() { return mEngine.get(); }
		ResourceManager* getResourceManager() { return mResourceManager.get(); }

	private:
		// order is valuable: engine and then resource manager 
		std::unique_ptr<Engine::Internal::Engine> mEngine;
		std::unique_ptr<ResourceManager> mResourceManager;
	};
}
