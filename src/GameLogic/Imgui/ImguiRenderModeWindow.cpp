#include "Base/precomp.h"

#include "GameLogic/Imgui/ImguiRenderModeWindow.h"

#include "imgui/imgui.h"

#include "GameData/GameData.h"
#include "GameData/Components/RenderModeComponent.generated.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

void ImguiRenderModeWindow::update(WorldHolder& worldHolder)
{
	GameData& gameData = worldHolder.getGameData();

	if (auto [renderMode] = gameData.getGameComponents().getComponents<RenderModeComponent>(); renderMode)
	{
		if (isVisible)
		{
			ImGui::Begin("Render modes", &isVisible);

			ImGui::Checkbox("Lights", &renderMode->getIsDrawLightsEnabledRef());
			ImGui::Checkbox("Visible Entities", &renderMode->getIsDrawVisibleEntitiesEnabledRef());
			ImGui::Checkbox("Debug AI", &renderMode->getIsDrawDebugAiDataEnabledRef());
			ImGui::Checkbox("Debug Cell Info", &renderMode->getIsDrawDebugCellInfoEnabledRef());
			ImGui::Checkbox("Debug Collisions", &renderMode->getIsDrawDebugCollisionsEnabledRef());
			ImGui::Checkbox("Debug Primitives", &renderMode->getIsDrawDebugPrimitivesEnabledRef());
			ImGui::Checkbox("Debug Character Info", &renderMode->getIsDrawDebugCharacterInfoEnabledRef());

			ImGui::End();
		}
	}
}
