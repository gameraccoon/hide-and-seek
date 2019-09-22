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

	mProfileSystems = arguments.hasArgument("profile-systems");
	mSystemProfileOutputPath = arguments.getArgumentValue("profile-systems", mSystemProfileOutputPath);

	initTestCase(arguments);

	// start the main loop
	getEngine()->start(this);
}

void BaseTestCase::update(float)
{
	constexpr float fixedDt = 0.16f;

	do
	{
		mTime.update(fixedDt);
		mSystemsManager.update();
		++mTicksCount;

		if (mProfileSystems)
		{
			mSystemFrameRecords.addFrame(mSystemsManager.getPreviousFrameTimeData());
		}
	}
	while (mOneFrame && mTicksCount < mTicksToFinish);

	if (mTicksCount >= mTicksToFinish)
	{
		finalizeTestCase();
		getEngine()->quit();
	}
}

void BaseTestCase::finalizeTestCase()
{
	if (mProfileSystems)
	{
		mSystemFrameRecords.printToFile(mSystemsManager.getSystemNames(), mSystemProfileOutputPath);
	}
}
