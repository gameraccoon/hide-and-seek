#pragma once

#include "GameLogic/Imgui/ImguiRenderModeWindow.h"
#include "GameLogic/Imgui/ImguiSystemsTimeReportWindow.h"

class ImguiMainMenu
{
public:
	void update(struct ImguiDebugData& debugData);

private:
	ImguiRenderModeWindow mRenderModeWindow;
	ImguiSystemsTimeReportWindow mSystemsTimeReportWindow;
};
