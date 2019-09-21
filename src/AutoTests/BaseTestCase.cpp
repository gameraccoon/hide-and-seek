#include "AutoTests/BaseTestCase.h"

#include <fstream>
#include <experimental/filesystem>
#include <memory>

#include "HAL/Base/Engine.h"

#include "GameLogic/ComponentsRegistration.h"

void BaseTestCase::start(const ArgumentsParser& arguments)
{
	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;
	mWorldHolder.gameData = &mGameData;

	mOneFrame = arguments.hasArgument("one-frame");

#ifdef PROFILE_SYSTEMS
	mProfileSystems = arguments.hasArgument("profile-systems");
	mSystemProfileOutputPath = arguments.getArgumentValue("profile-systems", mSystemProfileOutputPath);
	mSystemFrameRecords.reserve(mTicksToFinish);
#endif // PROFILE_SYSTEMS

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

#ifdef PROFILE_SYSTEMS
		mSystemFrameRecords.emplace_back(mSystemsManager.getLastFrameData());
#endif // PROFILE_SYSTEMS
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
#ifdef PROFILE_SYSTEMS
	if (mProfileSystems)
	{
		namespace fs = std::experimental::filesystem;
		fs::path outPath(mSystemProfileOutputPath);
		std::ofstream outFile(outPath);
		printSystemFrameRecords(outFile);
	}
#endif // PROFILE_SYSTEMS
}

#ifdef PROFILE_SYSTEMS
void BaseTestCase::printSystemFrameRecords(std::ostream& outStream)
{
	// printing in CSV format
	outStream << "\"Total\"";
	for (const auto& systemName : mSystemsManager.getSystemNames())
	{
		outStream << ",\"" << systemName << "\"";
	}
	outStream << "\n";

	for (const auto& frameRecord : mSystemFrameRecords)
	{
		outStream << frameRecord.frameTime.count();
		for (const auto& systemTime : frameRecord.systemTime)
		{
			outStream << "," << systemTime.count();
		}
		outStream << "\n";
	}
}
#endif // PROFILE_SYSTEMS
