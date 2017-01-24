#include "Game.h"

#include "Base/Engine.h"

Engine::Game::Game()
{
	mEngine.reset(new Engine::Internal::Engine());
	mResourceManager.reset(new ResourceManager(mEngine.get()));
}
