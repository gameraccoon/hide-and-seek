#include "Base/precomp.h"

#include "KeyStatesMap.h"

#include <algorithm>

namespace HAL
{
	void KeyStatesMap::updateState(int key, bool isPressed)
	{
		if (isPressed && mPressedKeys.find(key) == mPressedKeys.end())
		{
			mPressedKeys.insert(key);
			mLastFramePressedKeys.push_back(key);
		}
		else if (!isPressed)
		{
			mPressedKeys.erase(key);
			mLastFrameReleasedKeys.push_back(key);
		}
	}

	void KeyStatesMap::clearLastFrameState()
	{
		mLastFramePressedKeys.clear();
		mLastFrameReleasedKeys.clear();
	}

	bool KeyStatesMap::isPressed(int key) const
	{
		return mPressedKeys.find(key) != mPressedKeys.end();
	}

	bool KeyStatesMap::isJustPressed(int key) const
	{
		return std::find(mLastFramePressedKeys.begin(), mLastFramePressedKeys.end(), key) != mLastFramePressedKeys.end();
	}

	bool KeyStatesMap::isJustReleased(int key) const
	{
		return std::find(mLastFrameReleasedKeys.begin(), mLastFrameReleasedKeys.end(), key) != mLastFrameReleasedKeys.end();
	}
}
