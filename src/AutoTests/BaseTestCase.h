#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/ComponentFactory.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Application/ArgumentsParser.h"
#include "Utils/Profiling/SystemFrameRecords.h"
#include "Utils/Jobs/WorkerManager.h"

#include "HAL/GameBase.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/InputData.h"

class BaseTestCase : public HAL::GameBase
{
public:
	using HAL::GameBase::GameBase;

	void start(const ArgumentsParser& arguments);
	void update(float dt) final;
	void initResources() override;
	void setKeyboardKeyState(int, bool) override {}
	void setMouseKeyState(int, bool) override {}

protected:
	virtual void initTestCase(const ArgumentsParser& arguments) = 0;
	virtual void finalizeTestCase();

protected:
	World mWorld;
	GameData mGameData;
	WorldHolder mWorldHolder {&mWorld, mGameData};
	SystemsManager mSystemsManager;
	Jobs::WorkerManager mWorkerManager{1};
	TimeData mTime;
	InputData mInputData;

private:
	static const int mTicksToFinish = 100;
	int mTicksCount = 0;
	ComponentFactory mComponentFactory;
	bool mOneFrame = false;

	bool mProfileSystems = false;
	SystemFrameRecords mSystemFrameRecords;
	std::string mSystemProfileOutputPath = "systemProfile.csv";
};
