#pragma once

class WorldHolder;

class ImguiSystemsTimeReportWindow
{
public:
	void update(class ImguiDebugData& debugData);

	bool isVisible = false;

private:
	float mMaxTotalTimeMs = 10.0f;
	float mMaxSystemTimeMs = 1.0f;
};
