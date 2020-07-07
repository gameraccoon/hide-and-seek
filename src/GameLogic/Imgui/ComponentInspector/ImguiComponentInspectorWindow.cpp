#include "Base/precomp.h"

#include "GameLogic/Imgui/ComponentInspector/ImguiComponentInspectorWindow.h"

#include <cstring>
#include <sstream>
#include <string_view>
#include <string.h>

#include "imgui/imgui.h"

#include "ECS/ComponentFactory.h"

#include "GameData/GameData.h"
#include "GameData/World.h"

#include "GameLogic/Imgui/ImguiDebugData.h"
#include "GameLogic/SharedManagers/WorldHolder.h"

#include "GameLogic/Imgui/ComponentInspector/ComponentWidgetsRegistration.h"

ImguiComponentInspectorWindow::ImguiComponentInspectorWindow()
{
	ComponentWidgetsRegistration::RegisterInspectWidgets(mComponentInspectWidgets);
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ImguiComponentInspectorWindow::filterByComponents(ImguiDebugData& debugData)
{
	if (ImGui::CollapsingHeader("Filter By Components"))
	{
		ImGui::Columns(3, NULL, false);
		debugData.componentFactory.forEachComponentType([this](std::type_index typeID, StringID className){
			if (ImGui::Selectable(ID_TO_STR(className).c_str(), &mComponentFilters[typeID])) {}
			ImGui::NextColumn();
		});
		ImGui::Columns(1);
		ImGui::Separator();
	}
}

void ImguiComponentInspectorWindow::filterByProperties(ImguiDebugData& /*debugData*/)
{
	if (ImGui::CollapsingHeader("Filter By Properties"))
	{
		mPropertyFiltersWidget.update();
	}
}

void ImguiComponentInspectorWindow::applyFilters(ImguiDebugData& debugData)
{
	std::vector<std::type_index> filteredComponents;
	for (const auto [typeID, isEnabled] : mComponentFilters)
	{
		if (isEnabled) { filteredComponents.push_back(typeID); }
	}
	mPropertyFiltersWidget.appendFilteredComponentTypes(filteredComponents);

	SpatialEntityManager allManagers = debugData.worldHolder.getWorld().getSpatialData().getAllCellManagers();

	mFilteredEntities.clear();
	allManagers.getSpatialEntitiesHavingComponents(filteredComponents, mFilteredEntities);

	mPropertyFiltersWidget.filterEntities(mFilteredEntities);
}

void ImguiComponentInspectorWindow::processEntityIDInput(ImguiDebugData& debugData)
{
	if (ImGui::InputTextWithHint("Entity ID Filter", "Entity ID", mEntityFilterBuffer, IM_ARRAYSIZE(mEntityFilterBuffer)))
	{
		Entity::EntityID id = 0;
		std::string_view strId(mEntityFilterBuffer, IM_ARRAYSIZE(mEntityFilterBuffer));
		std::stringstream ss;
		if (strId[0] == '0' && strId[1] == 'x') {
			ss << std::hex;
		}
		ss << strId;
		ss >> id;
		Entity entity(id);

		SpatialEntityManager allManagers = debugData.worldHolder.getWorld().getSpatialData().getAllCellManagers();
		WorldCell* cell = allManagers.findEntityCell(entity);
		if (cell != nullptr)
		{
			mSelectedEntity = std::make_tuple(cell, entity);
		}
		else
		{
			mSelectedEntity = std::nullopt;
		}
	}
}

void ImguiComponentInspectorWindow::showFilteredEntities()
{
	if (!mFilteredEntities.empty())
	{
		if (ImGui::TreeNode("Filtered entities"))
		{
			ImGui::BeginGroup();
			auto scrollBoxSize = ImVec2(200.0f, std::min(180.0f, mFilteredEntities.size() * 17.0f + ImGui::GetStyle().FramePadding.y*4));
			if (ImGui::BeginChild("FilteredEntities", scrollBoxSize, true))
			{
				for (int i = 0, iSize = mFilteredEntities.size(); i < iSize; ++i)
				{
					const Entity& entity = std::get<1>(mFilteredEntities[i]);
					char buf[32];
					sprintf(buf, "0x%x", entity.getID());
					if (ImGui::Selectable(buf, mSelectedEntity.has_value() && std::get<1>(*mSelectedEntity) == entity))
					{
						mSelectedEntity = mFilteredEntities[i];
						std::strcpy(mEntityFilterBuffer, buf);
					}
				}
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			ImGui::TreePop();
		}
	}
}

void ImguiComponentInspectorWindow::showComponentsInspector()
{
	bool hasFoundAnything = false;
	if (mSelectedEntity.has_value())
	{
		auto [cell, entity] = *mSelectedEntity;

		std::vector<BaseComponent*> components;
		cell->getEntityManager().getAllEntityComponents(entity, components);
		std::sort(components.begin(), components.end(), [](auto a, auto b){
			return a->getComponentTypeName() < b->getComponentTypeName();
		});
		for (BaseComponent* component : components)
		{
			std::string name = FormatString("%s##ComponentInspection", ID_TO_STR(component->getComponentTypeName()).c_str());
			if (ImGui::TreeNode(name.c_str()))
			{
				auto it = mComponentInspectWidgets.find(component->getComponentTypeName());
				if (it != mComponentInspectWidgets.end())
				{
					it->second->update(component);
				}

				ImGui::TreePop();
				ImGui::Separator();
			}
			hasFoundAnything = true;
		}
	}

	if (!hasFoundAnything && mSelectedEntity.has_value())
	{
		ImGui::Text("No inspectable entity with such ID found");
		ImGui::SameLine(); HelpMarker("An entity without any components also can't be inspectable");
	}
}

void ImguiComponentInspectorWindow::update(ImguiDebugData& debugData)
{
	if (isVisible)
	{
		ImGui::Begin("Component Inspector", &isVisible);

		filterByComponents(debugData);
		filterByProperties(debugData);

		applyFilters(debugData);

		ImGui::Text("Entities matching the filter: %lu", mFilteredEntities.size());

		showFilteredEntities();
		processEntityIDInput(debugData);
		showComponentsInspector();

		ImGui::End();
	}
}
