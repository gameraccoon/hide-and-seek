#pragma once

#include "Game.h"

#include <Core/World.h>
#include <Utils/Application/ArgumentsParser.h>
#include <Utils/Core/SystemsManager.h>
#include <Utils/Core/ComponentFactory.h>

namespace Game
{
	class StealthGame : public SystemInterface::Game
	{
	public:
		StealthGame() = default;

		void start(ArgumentsParser& arguments);
		virtual void update(float dt) override;
		virtual void setKeyState(int key, bool isPressed) override;

	private:
		World mWorld;
		std::unordered_map<int, bool> mKeyStates;
		SystemsManager mSystemsManager;
		ComponentFactory mComponentFactory;
	};
}
