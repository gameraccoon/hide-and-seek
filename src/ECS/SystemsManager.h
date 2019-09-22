#pragma once

#include <vector>
#include <memory>
#include "ECS/System.h"

#include <chrono>

#ifdef DEBUG
	#define PROFILE_SYSTEMS
#endif // DEBUG

#ifdef PROFILE_SYSTEMS
struct SystemsFrameTime
{
	std::chrono::microseconds frameTime;
	std::vector<std::chrono::microseconds> systemTime;
};
#endif // PROFILE_SYSTEMS

/**
 * Manager for game systems
 */
class SystemsManager
{
public:
	template <typename T, typename... Args>
	void registerSystem(Args&&... args)
	{
		mSystems.emplace_back(new T(std::forward<Args>(args)...));
	}

	void update();

	SystemsFrameTime getPreviousFrameTimeData();
	std::vector<std::string> getSystemNames();

private:
	std::vector<std::unique_ptr<System>> mSystems;
	SystemsFrameTime mThisFrameTime;
	SystemsFrameTime mPreviousFrameTime;
};
