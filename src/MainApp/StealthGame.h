#pragma once

#include "EngineInterface/Internal/Engine.h"
#include "EngineInterface/Game.h"

namespace Game {
	class StealthGame : public Engine::Game
	{
	public:
		void Start();
		virtual void Update(float dt) override;
		virtual void Draw() override;
	};
}
