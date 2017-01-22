#pragma once

#include "Game.h"

#include <Core/World.h>

namespace Game
{
	class StealthGame : public Engine::Game
	{
	public:
		void start();
		virtual void update(float dt) override;
		virtual void draw() override;

	private:
		std::unique_ptr<World> mWorld;
	};
}
