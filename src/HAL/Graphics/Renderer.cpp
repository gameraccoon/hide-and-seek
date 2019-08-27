#include "HAL/Graphics/Renderer.h"

#include  "SDL_FontCache/SDL_FontCache.h"

#include "HAL/Internal/GlContext.h"

#include "HAL/Internal/SdlWindow.h"
#include "HAL/Internal/SdlSurface.h"
#include "HAL/Graphics/Font.h"
#include "HAL/Graphics/Texture.h"


namespace Graphics
{
	Renderer::Renderer(HAL::Internal::Window& window)
	{
		mRenderer = SDL_CreateRenderer(window.getRawWindow(), -1, SDL_RENDERER_PRESENTVSYNC);
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(mRenderer);
	}

	void Renderer::clearFrame(Graphics::Color color)
	{
		SDL_SetRenderDrawColor(mRenderer, color.R, color.G, color.B, color.A);
		SDL_RenderClear(mRenderer);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size)
	{
		SDL_Rect uvRect = {
			static_cast<int>(0),
			static_cast<int>(0),
			static_cast<int>(texture.getWidth()),
			static_cast<int>(texture.getHeight())
		};
		SDL_Rect outRect = {
			static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(size.x),
			static_cast<int>(size.y)
		};
		SDL_RenderCopy(mRenderer, texture.getRawTexture(), &uvRect, &outRect);
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, float alpha)
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, static_cast<Uint8>(alpha * 255.0f));
		render(texture, pos, size);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation)
	{
		render(texture, pos, size, ancor, rotation, {0.f, 0.f, 1.f, 1.f});
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, float alpha)
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, static_cast<Uint8>(alpha * 255.0f));
		render(texture, pos, size, ancor, rotation);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, Graphics::QuadUV uv)
	{
		SDL_Rect uvRect = {
			static_cast<int>(uv.U1 * texture.getWidth()),
			static_cast<int>(uv.V1 * texture.getHeight()),
			static_cast<int>((uv.U2 - uv.U1) * texture.getWidth()),
			static_cast<int>((uv.V2 - uv.V1) * texture.getHeight())
		};
		SDL_Rect outRect = {
			static_cast<int>(pos.x - ancor.x * size.x),
			static_cast<int>(pos.y - ancor.y * size.y),
			static_cast<int>(size.x),
			static_cast<int>(size.y)
		};
		SDL_Point ancorPoint = {
			static_cast<int>(ancor.x * texture.getWidth()),
			static_cast<int>(ancor.y * texture.getHeight())
		};
		SDL_RenderCopyEx(mRenderer, texture.getRawTexture(), &uvRect, &outRect, rotation, &ancorPoint, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	void Renderer::render(const Graphics::Texture& texture, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, Graphics::QuadUV uv, float alpha)
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, static_cast<Uint8>(alpha * 255.0f));
		render(texture, pos, size, ancor, rotation, uv);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	}

	void Renderer::renderText(const Graphics::Font& font, Vector2D pos, Graphics::Color color, const char* text)
	{
		FC_DrawColor(font.getRawFont(), mRenderer, pos.x, pos.y, { color.R, color.G, color.B, color.A }, text);
	}

	std::array<int, 2> Renderer::getTextSize(const Graphics::Font& font, const char* text)
	{
		return { FC_GetWidth(font.getRawFont(), text), FC_GetHeight(font.getRawFont(), text) };
	}

	void Renderer::finalizeFrame()
	{
		SDL_RenderPresent(mRenderer);
	}

	SDL_Renderer* Renderer::getRawRenderer()
	{
		return mRenderer;
	}
}
