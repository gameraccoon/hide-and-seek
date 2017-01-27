#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "Graphics/Texture.h"


namespace Internal
{
	class SdlSurface;
}

namespace SystemInterface
{
	class IGame;

	class Engine
	{
	public:
		Engine();
		~Engine();

		float getMouseX() const;
		float getMouseY() const;
		bool getMouseButtonDown() const;

		void start(IGame& game);
		void quit();

		void render(Internal::SdlSurface* surface, const glm::mat4& transform, float alpha = 1.0f);
		void render(Internal::SdlSurface* surface, float x, float y, float rotation = 0.0f, float alpha = 1.0f);

		float calculateStringWidth(const char* text) const;
		void write(Internal::SdlSurface* fontSurface, const char* text, const glm::mat4& transform);
		void write(Internal::SdlSurface* fontSurface, const char* text, float x, float y, float rotation = 0.0f);

		int getWidth() const;
		int getHeight() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mPimpl;
	};
}
