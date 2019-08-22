#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "GameData/Core/Vector2D.h"

#include "HAL/Base/Types.h"

namespace HAL
{
	namespace Internal
	{
		class SdlSurface;
	}

	class IGame;

	struct DrawPoint
	{
		Vector2D spacePoint;
		Vector2D texturePoint;
	};

	class Engine
	{
	public:
		Engine();
		~Engine();

		float getMouseX() const;
		float getMouseY() const;
		bool getMouseButtonDown() const;

		bool getKeyState(char key);

		void start(IGame* game);
		void quit();

		void render(Internal::SdlSurface* surface, const glm::mat4& transform, Vector2D size, Graphics::QuadUV uv, float alpha = 1.0f);
		void render(Internal::SdlSurface* surface, Vector2D pos, Vector2D size, Vector2D ancor, Graphics::QuadUV uv, float rotation = 0.0f, float alpha = 1.0f);
		void renderFan(Internal::SdlSurface* surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha);
		void renderStrip(Internal::SdlSurface* surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha);

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
