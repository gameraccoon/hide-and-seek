#pragma once

#include <memory>
#include <vector>

#ifdef IMGUI_ENABLED
#include "ECS/System.h"

#include "HAL/EngineFwd.h"

#include "GameLogic/SharedManagers/WorldHolder.h"
#include "GameLogic/SharedManagers/TimeData.h"

#include "GameLogic/Imgui/ImguiMainMenu.h"


/**
 * System that handles dear imgui debug tool
 */
class ImguiSystem : public System
{
public:
	using KeyStatesMap = std::unordered_map<int, bool>;

public:
	ImguiSystem(WorldHolder& worldHolder, const TimeData& timeData, HAL::Engine& engine);
	~ImguiSystem() override = default;

	void update() override;
	void initResources() override;
	void shutdown() override;
	std::string getName() override { return "ImguiSystem"; }

private:
	WorldHolder& mWorldHolder;
	const TimeData& mTime;
	HAL::Engine& mEngine;

	ImguiMainMenu mImguiMainMenu;
};

#endif // IMGUI_ENABLED
