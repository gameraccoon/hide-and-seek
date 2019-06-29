#include "HAL/GameBase.h"

#include "Base/Engine.h"

namespace HAL
{
	GameBase::GameBase()
		: mEngine(new Engine())
	{
		mResourceManager.reset(new ResourceManager(mEngine.get()));
	}

	GameBase::~GameBase()
	{
	}
}
