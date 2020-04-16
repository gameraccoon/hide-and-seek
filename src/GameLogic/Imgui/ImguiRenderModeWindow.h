#pragma once

class WorldHolder;

class ImguiRenderModeWindow
{
public:
	void update(WorldHolder& worldHolder);

	bool isVisible = false;
};
