#pragma once

struct Vector2D;
class Rotator;
class StringID;

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
	void Inspector(const char* title, Vector2D& data);

	template<>
	void Inspector(const char* title, Rotator& data);

	template<>
	void Inspector(const char* title, StringID& data);
}
