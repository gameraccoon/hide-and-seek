#include "Game.h"

#include "Base/Engine.h"
namespace SystemInterface
{
	Game::Game()
		: mEngine(new Engine())
	{
		mResourceManager.reset(new ResourceManager(mEngine.get()));
	}

	Game::~Game()
	{
	}
}
