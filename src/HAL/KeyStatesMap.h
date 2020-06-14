#pragma once

#include <unordered_set>
#include <vector>

namespace HAL
{
	class KeyStatesMap
	{
	public:
		void updateState(int key, bool isPressed);
		void clearLastFrameState();
		bool isPressed(int key) const;
		bool isJustPressed(int key) const;
		bool isJustReleased(int key) const;

	private:
		std::unordered_set<int> mPressedKeys;
		std::vector<int> mLastFramePressedKeys;
		std::vector<int> mLastFrameReleasedKeys;
	};
}
