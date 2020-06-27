#pragma once

#include "ECS/Entity.h"
#include <map>

class WorldHolder;

class ImguiComponentInspectorWindow
{
public:
	void update(struct ImguiDebugData& debugData);

	bool isVisible = false;

private:
	std::map<std::type_index, bool> mComponentFilters;
	char mEntityFilterBuffer[128] = "";
	OptionalEntity mSelectedFilteredEntity;
};
