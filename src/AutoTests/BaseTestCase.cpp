#include "AutoTests/BaseTestCase.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "GameLogic/ComponentsRegistration.h"

void BaseTestCase::start(const ArgumentsParser& arguments)
{
	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;
	mWorldHolder.gameData = &mGameData;

	mOneFrame = arguments.hasArgument("one-frame");

	initTestCase(arguments);

	// start the main loop
	getEngine()->start(this);
}

void BaseTestCase::update(float)
{
	constexpr float fixedDt = 0.16f;

	if (mOneFrame)
	{
		for (int i = mTicksCount; i < mTicksToFinish - 1; ++i)
		{
			mTime.update(fixedDt);
			mSystemsManager.update();
		}
		mTicksCount = mTicksToFinish - 1;
	}

	mTime.update(fixedDt);
	mSystemsManager.update();
	++mTicksCount;

	if (mTicksCount >= mTicksToFinish)
	{
		getEngine()->quit();
	}
}
