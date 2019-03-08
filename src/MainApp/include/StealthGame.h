#pragma once

#include "Game.h"

#include <Core/World.h>
#include <Graphics/Camera.h>
#include <Modules/SystemsManager.h>

namespace Game
{
	class StealthGame : public SystemInterface::Game
	{
	public:
		StealthGame() = default;

		void start();
		virtual void update(float dt) override;
		virtual void setKeyState(int key, bool isPressed) override;

	private:
		std::unique_ptr<World> mWorld;
		std::unordered_map<int, bool> mKeyStates;
		std::unique_ptr<SystemsManager> mSystemsManager;
	};
}
