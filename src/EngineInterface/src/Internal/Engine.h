#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "Graphics/Texture.h"

namespace Engine
{
	class IGame;

	namespace Internal
	{
		class Engine
		{
		public:
			Engine();
			~Engine();

			float getMouseX() const;
			float getMouseY() const;
			bool getMouseButtonDown() const;

			void start(IGame& updater);
			void quit();

			void render(SdlSurface* surface, const glm::mat4& transform, float alpha = 1.0f);
			void render(SdlSurface* surface, float x, float y, float rotation = 0.0f, float alpha = 1.0f);

			float calculateStringWidth(const char* text) const;
			void write(SdlSurface* fontSurface, const char* text, const glm::mat4& transform);
			void write(SdlSurface* fontSurface, const char* text, float x, float y, float rotation = 0.0f);

			int getWidth() const;
			int getHeight() const;

		private:
			struct Impl;
			std::unique_ptr<Impl> mPimpl;
		};
	}
}
