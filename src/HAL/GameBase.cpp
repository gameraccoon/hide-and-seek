#include "HAL/GameBase.h"

#include "HAL/Base/Engine.h"

namespace HAL
{
	GameBase::GameBase(int windowWidth, int windowHeight)
		: mEngine(new Engine(windowWidth, windowHeight))
		, mResourceManager(new ResourceManager(mEngine.get()))
	{
	}

	GameBase::~GameBase() = default;
}
