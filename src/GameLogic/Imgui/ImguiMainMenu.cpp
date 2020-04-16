#include "Base/precomp.h"

#include "GameLogic/Imgui/ImguiMainMenu.h"

#include "imgui/imgui.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

void ImguiMainMenu::update(WorldHolder& worldHolder)
{
	ImGui::Begin("Debug Menu");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", static_cast<double>(1000.0f / ImGui::GetIO().Framerate), static_cast<double>(ImGui::GetIO().Framerate));

	if (ImGui::Button("Render Modes"))
	{
		mRenderModeWindow.isVisible = !mRenderModeWindow.isVisible;
	}

	ImGui::End();

	mRenderModeWindow.update(worldHolder);
}
