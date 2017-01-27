#pragma once

#include "Base/ResourceManager.h"

namespace SystemInterface
{
	class IGame
	{
	public:
		virtual void update(float dt) = 0;
		virtual void draw() = 0;
	};
}
