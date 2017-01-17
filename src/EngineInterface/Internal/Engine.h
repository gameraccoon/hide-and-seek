#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "../Graphics/Texture.h"

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

			float GetMouseX() const;
			float GetMouseY() const;
			bool GetMouseButtonDown() const;

			void Start(IGame& updater);
			void Quit();

			void Render(SdlSurface* surface, const glm::mat4& transform, float alpha = 1.0f);
			void Render(SdlSurface* surface, float x, float y, float rotation = 0.0f, float alpha = 1.0f);

			float CalculateStringWidth(const char* text) const;
			void Write(SdlSurface* fontSurface, const char* text, const glm::mat4& transform);
			void Write(SdlSurface* fontSurface, const char* text, float x, float y, float rotation = 0.0f);

			int GetWidth() const;
			int GetHeight() const;

		private:
			struct Impl;
			std::unique_ptr<Impl> mPimpl;
		};
	}
}
