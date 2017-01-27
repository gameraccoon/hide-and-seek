#include "Game.h"

#include "Base/Engine.h"
namespace SystemInterface
{
	Game::Game()
	{
		mEngine.reset(new Engine());
		mResourceManager.reset(new ResourceManager(mEngine.get()));
	}

	Game::~Game()
	{
	}
}