#pragma once

#include <unordered_set>
#include <vector>

namespace SystemInterface
{
	class KeyStatesMap
	{
	public:
		void updateState(int key, bool isPressed);
		void clearLastFrameState();
		bool isPressed(int key);
		bool isJustPressed(int key);
		bool isJustReleased(int key);

	private:
		std::unordered_set<int> mPressedKeys;
		std::vector<int> mLastFramePressedKeys;
		std::vector<int> mLastFrameReleasedKeys;
	};
}
