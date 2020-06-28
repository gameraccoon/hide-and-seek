#pragma once

#include "ECS/Entity.h"
#include <map>

#include "GameLogic/Imgui/ComponentInspector/AbstractComponentImguiWidget.h"

class WorldHolder;

class ImguiComponentInspectorWindow
{
public:
	ImguiComponentInspectorWindow();

	void update(struct ImguiDebugData& debugData);

	bool isVisible = false;

private:
	void filterByComponents(ImguiDebugData& debugData);
	void filterByProperties(ImguiDebugData& debugData);

	void showEntityID();
	void showFilteredEntities();
	void showComponentsInspector(ImguiDebugData& debugData);

private:
	std::map<std::type_index, bool> mComponentFilters;
	char mEntityFilterBuffer[128] = "";
	OptionalEntity mSelectedEntity;
	std::vector<Entity> mFilteredEntities;
	std::map<StringID, std::unique_ptr<AbstractComponentImguiWidget>> mComponentInspectWidgets;
};
