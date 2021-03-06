#include "Base/precomp.h"

#include "AutoTests/BaseTestCase.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "GameData/ComponentRegistration/ComponentFactoryRegistration.h"

TestChecklist BaseTestCase::start(const ArgumentsParser& arguments)
{
	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mOneFrame = arguments.hasArgument("one-frame");

	mProfileSystems = arguments.hasArgument("profile-systems");
	mSystemProfileOutputPath = arguments.getArgumentValue("profile-systems", mSystemProfileOutputPath);

	initTestCase(arguments);

	// start the main loop
	getEngine().start(this);

	return std::move(mTestChecklist);
}

void BaseTestCase::update(float)
{
	constexpr float fixedDt = 1.0f / 60.0f;

	do
	{
		mTime.update(fixedDt);
		mSystemsManager.update();
		++mTicksCount;
#ifdef PROFILE_SYSTEMS
		if (mProfileSystems)
		{
			mSystemFrameRecords.addFrame(mSystemsManager.getPreviousFrameTimeData());
		}
#endif // PROFILE_SYSTEMS
	}
	while (mOneFrame && mTicksCount < mTicksToFinish);

	if (mTicksCount >= mTicksToFinish)
	{
		finalizeTestCase();
		getEngine().quit();
	}
}

void BaseTestCase::initResources()
{
	getResourceManager().loadAtlasesData("resources/atlas/atlas-list.json");
	mSystemsManager.initResources();
}

void BaseTestCase::finalizeTestCase()
{
	if (mProfileSystems)
	{
		mSystemFrameRecords.printToFile(mSystemsManager.getSystemNames(), mSystemProfileOutputPath);
	}
}
