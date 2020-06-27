#include "Base/precomp.h"

#include "GameLogic/Imgui/ComponentInspector/ImguiComponentInspectorWindow.h"

#include <sstream>
#include <string_view>
#include <string.h>

#include "imgui/imgui.h"

#include "GameData/GameData.h"
#include "GameData/World.h"

#include "GameLogic/Imgui/ImguiDebugData.h"

void ImguiComponentInspectorWindow::update(ImguiDebugData& debugData)
{
	if (isVisible)
	{
		ImGui::Begin("Component Inspector", &isVisible);

		if (ImGui::CollapsingHeader("Filter By Components"))
		{
			ImGui::Columns(3, NULL, false);
			debugData.componentFactory.forEachComponentType([this](std::type_index typeID, StringID className){
				if (ImGui::Selectable(ID_TO_STR(className).c_str(), &mComponentFilters[typeID])) {}
				ImGui::NextColumn();
			});
			ImGui::Columns(1);
			ImGui::Separator();

			std::vector<std::type_index> filteredComponents;
			for (const auto [typeID, isEnabled] : mComponentFilters)
			{
				if (isEnabled) { filteredComponents.push_back(typeID); }
			}

			SpatialEntityManager allManagers = debugData.worldHolder.getWorld().getSpatialData().getAllCellManagers();

			std::vector<Entity> filteredEntities;
			allManagers.getEntitiesHavingComponents(filteredComponents, filteredEntities);

			ImGui::Text("Entities matching the filter: %lu", filteredEntities.size());

			ImGui::BeginGroup();
			auto scrollBoxSize = ImVec2(200.0f, std::min(180.0f, filteredEntities.size() * 17.0f + ImGui::GetStyle().FramePadding.y*4));
			if (ImGui::BeginChild("FilteredEntities", scrollBoxSize, true))
			{
				for (int i = 0, iSize = filteredEntities.size(); i < iSize; ++i)
				{
					const Entity& entity = filteredEntities[i];
					char buf[32];
					sprintf(buf, "0x%x", entity.getID());
					if (ImGui::Selectable(buf, mSelectedFilteredEntity.isValid() && mSelectedFilteredEntity.getEntity() == entity))
					{
						mSelectedFilteredEntity = entity;
						std::strcpy(mEntityFilterBuffer, buf);
					}
				}
			}
			ImGui::EndChild();
			ImGui::EndGroup();
		}

		if (ImGui::CollapsingHeader("Inspect Entity Components"))
		{
			ImGui::InputTextWithHint("Entity ID Filter", "Entity ID", mEntityFilterBuffer, IM_ARRAYSIZE(mEntityFilterBuffer));
//			ImGui::SameLine(); HelpMarker("Help description");

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

			std::vector<BaseComponent*> components;
			allManagers.getAllEntityComponents(entity, components);
			for (BaseComponent* component : components)
			{
				std::string name = FormatString("%s##ComponentInspection", ID_TO_STR(component->getComponentTypeName()).c_str());
				if (ImGui::TreeNode(name.c_str()))
				{
					ImGui::Text("test");
					ImGui::TreePop();
					ImGui::Separator();
				}
			}
		}

		ImGui::End();
	}
}
