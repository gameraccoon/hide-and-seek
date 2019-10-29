#pragma once

#include "HAL/Base/ResourceManager.h"

namespace HAL
{
	class IGame
	{
	public:
		virtual ~IGame() = default;
		virtual void update(float dt) = 0;
		virtual void initResources() = 0;
		virtual void setKeyboardKeyState(int key, bool isPressed) = 0;
		virtual void setMouseKeyState(int key, bool isPressed) = 0;
	};
}
