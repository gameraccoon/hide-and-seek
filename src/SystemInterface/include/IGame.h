#pragma once

#include "Base/ResourceManager.h"

namespace SystemInterface
{
	class IGame
	{
	public:
		virtual ~IGame() = default;
		virtual void update(float dt) = 0;
		virtual void setKeyState(int key, bool isPressed) = 0;
	};
}
