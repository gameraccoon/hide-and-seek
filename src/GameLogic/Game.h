#pragma once

#include "ECS/SystemsManager.h"
#include "ECS/Serialization/ComponentSerializersHolder.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "Utils/Application/ArgumentsParser.h"
#include "Utils/Profiling/SystemFrameRecords.h"
#include "Utils/Jobs/WorkerManager.h"

#include "HAL/GameBase.h"

#include "GameLogic/SharedManagers/TimeData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/InputData.h"

#ifdef IMGUI_ENABLED
#include "GameLogic/Imgui/ImguiDebugData.h"
#endif

class Game : public HAL::GameBase
{
public:
	using HAL::GameBase::GameBase;

	void start(ArgumentsParser& arguments);
	void update(float dt) override;
	void setKeyboardKeyState(int key, bool isPressed) override;
	void setMouseKeyState(int key, bool isPressed) override;
	void initResources() override;

private:
	void initSystems();
	void onGameShutdown();

private:
	World mWorld;
	GameData mGameData;
	WorldHolder mWorldHolder {&mWorld, mGameData};

	InputData mInputData;

	SystemsManager mSystemsManager;
	ComponentSerializersHolder mComponentSerializers;
	Jobs::WorkerManager mJobsWorkerManager{Jobs::GetAvailableThreadsCount()};
	TimeData mTime;

#ifdef PROFILE_SYSTEMS
	bool mProfileSystems = false;
	std::string mSystemProfileOutputPath = "systemProfile.csv";
#endif // PROFILE_SYSTEMS

#if defined(IMGUI_ENABLED) || defined(PROFILE_SYSTEMS)
	SystemFrameRecords mSystemFrameRecords;
#endif // IMGUI_ENABLED || PROFILE_SYSTEMS

#ifdef IMGUI_ENABLED
	ImguiDebugData mImguiDebugData{mWorldHolder, mTime, mSystemFrameRecords, mComponentSerializers.factory, {}};
#endif // IMGUI_ENABLED
};
