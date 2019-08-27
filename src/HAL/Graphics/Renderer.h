#pragma once

#include <array>

#include "GameData/Core/Vector2D.h"

#include "HAL/Base/Types.h"

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

	class Renderer
	{
	public:
		Renderer(HAL::Internal::Window& window);
		~Renderer();

		void clearFrame(Graphics::Color color);

		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size);
		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, float alpha);
		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation);
		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, float alpha);
		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, QuadUV uv);
		void render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, QuadUV uv, float alpha);

		void renderText(const Font& font, Vector2D pos, Graphics::Color color, const char* text);
		std::array<int, 2> getTextSize(const Font& font, const char* text);

		void finalizeFrame();

		SDL_Renderer* getRawRenderer();

	private:
		SDL_Renderer* mRenderer;
	};
}
