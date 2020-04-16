#pragma once

#include "GameLogic/Imgui/ImguiRenderModeWindow.h"

class WorldHolder;

class ImguiMainMenu
{
public:
	void update(WorldHolder& worldHolder);

private:
	ImguiRenderModeWindow mRenderModeWindow;
};
