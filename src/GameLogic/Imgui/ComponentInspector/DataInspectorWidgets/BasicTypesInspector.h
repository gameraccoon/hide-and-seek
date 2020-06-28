#pragma once

namespace ImguiDataInspection
{
	template<typename T>
	void Inspector(const char* title, T& /*data*/)
	{
		ImGui::Text("Type inspection for '%s' is not yet implemented", title);
	}

	template<>
	void Inspector(const char* title, bool& data);

	template<>
	void Inspector(const char* title, int& data);

	template<>
	void Inspector(const char* title, float& data);

	template<>
	void Inspector(const char* title, class Vector2D& data);

	template<>
	void Inspector(const char* title, class Rotator& data);

	template<>
	void Inspector(const char* title, class StringID& data);
}
