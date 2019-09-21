#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Application/ArgumentsParser.h"

#include "HAL/GameBase.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

class BaseTestCase : public HAL::GameBase
{
public:
	using HAL::GameBase::GameBase;

	void start(const ArgumentsParser& arguments);
	void update(float dt) final;

protected:
	virtual void initTestCase(const ArgumentsParser& arguments) = 0;
	virtual void finalizeTestCase();

protected:
	WorldHolder mWorldHolder;
	World mWorld;
	GameData mGameData;
	SystemsManager mSystemsManager;
	TimeData mTime;

private:
	static const int mTicksToFinish = 100;
	int mTicksCount = 0;
	ComponentFactory mComponentFactory;
	bool mOneFrame = false;

#ifdef PROFILE_SYSTEMS
private:
	void printSystemFrameRecords(std::ostream& outStream);
	bool mProfileSystems = false;
	std::string mSystemProfileOutputPath = "systemProfile.csv";
	std::vector<SystemsFrameTime> mSystemFrameRecords;
#endif // PROFILE_SYSTEMS
};
