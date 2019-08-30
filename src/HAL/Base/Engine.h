#pragma once

#include <memory>

#include "HAL/Graphics/Renderer.h"
#include "HAL/Base/Types.h"

namespace HAL
{

	class IGame;

	class Engine
	{
	public:
		Engine(int windowWidth, int windowHeight);
		~Engine();

		float getMouseX() const;
		float getMouseY() const;

		void start(IGame* game);
		void quit();

		Graphics::Renderer* getRenderer();

		int getWidth() const;
		int getHeight() const;

	private:
		const int WindowWidth;
		const int WindowHeight;

		struct Impl;
		std::unique_ptr<Impl> mPimpl;
	};
}
