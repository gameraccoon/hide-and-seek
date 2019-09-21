#include "SystemsManager.h"

#include "Debug/Log.h"

void SystemsManager::update()
{
#ifdef PROFILE_SYSTEMS
	mLastFrameTime.frameTime = std::chrono::microseconds::zero();
	mLastFrameTime.systemTime.clear();
#endif // PROFILE_SYSTEMS

	for (std::unique_ptr<System>& system : mSystems)
	{
#ifdef PROFILE_SYSTEMS
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
#endif // PROFILE_SYSTEMS

		// real work is done here
		system->update();

#ifdef PROFILE_SYSTEMS
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		mLastFrameTime.frameTime += timeDiff;
		mLastFrameTime.systemTime.push_back(timeDiff);
#endif // PROFILE_SYSTEMS
	}
}

std::vector<std::string> SystemsManager::getSystemNames()
{
	std::vector<std::string> result;
	result.reserve(mSystems.size());
	for (std::unique_ptr<System>& system : mSystems)
	{
		result.push_back(system->getName());
	}
	return result;
}

#ifdef PROFILE_SYSTEMS
SystemsFrameTime SystemsManager::getLastFrameData()
{
	return mLastFrameTime;
}
#endif // PROFILE_SYSTEMS
