#pragma once

#include <array>

#include <glm/fwd.hpp>

#include "GameData/Core/Vector2D.h"

#include "HAL/Base/Types.h"
#include "HAL/EngineFwd.h"

struct SDL_Renderer;

namespace HAL
{
	namespace Internal
	{
		class Window;
	}
}

namespace Graphics
{
	class Font;
	class Texture;

	struct DrawPoint
	{
		Vector2D spacePoint;
		Vector2D texturePoint;
	};

	namespace Render
	{
		void drawQuad(const Internal::Surface& surface, const glm::mat4& transform, Vector2D size, QuadUV uv, float alpha = 1.0f);
		void drawQuad(const Internal::Surface& surface, Vector2D pos, Vector2D size);
		void drawQuad(const Internal::Surface& surface, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, QuadUV uv, float alpha = 1.0f);
		void drawFan(const Internal::Surface& surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha);
		void drawStrip(const Internal::Surface& surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha);
		void drawTiledQuad(const Internal::Surface& surface, Vector2D start, Vector2D size, Vector2D tiles, Vector2D uvShift);
	}

	class Renderer
	{
	public:
		Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		void renderText(const Font& font, Vector2D pos, Color color, const char* text);
		std::array<int, 2> getTextSize(const Font& font, const char* text);
	};
}
