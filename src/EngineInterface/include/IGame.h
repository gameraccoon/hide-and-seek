#pragma once

#include "Base/ResourceManager.h"

namespace Engine
{
	class IGame
	{
	public:
		virtual void update(float dt) = 0;
		virtual void draw() = 0;
	};
}
