#pragma once

#include "Internal/Engine.h"
#include "Graphics/ResourceManager.h"

namespace Engine
{
	class IGame
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void Draw() = 0;
	};
}
