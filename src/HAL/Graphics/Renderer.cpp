#include "Base/precomp.h"

#include "HAL/Graphics/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>

#include "sdl/SDL_surface.h"

#include "HAL/Internal/GlContext.h"

#include "HAL/Internal/SdlWindow.h"
#include "HAL/Internal/SdlSurface.h"
#include "HAL/Graphics/Font.h"
#include "HAL/Internal/SdlSurface.h"


static constexpr double MATH_PI = 3.14159265358979323846;

namespace Graphics
{
	void Render::drawQuad(const Internal::Surface& surface, const glm::mat4& transform, Vector2D size, Graphics::QuadUV uv, float alpha)
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface.bind();

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		glTexCoord2f(uv.U1, uv.V2); glVertex2f(0.0f, size.y);
		glTexCoord2f(uv.U2, uv.V2); glVertex2f(size.x, size.y);
		glTexCoord2f(uv.U2, uv.V1); glVertex2f(size.x, 0.0f);
		glTexCoord2f(uv.U1, uv.V1); glVertex2f(0.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}

	void Render::drawQuad(const Internal::Surface& surface, Vector2D pos, Vector2D size)
	{
		glm::mat4 transform{ 1.0f };
		transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface.bind();

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, size.y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(size.x, size.y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(size.x, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}

	void Render::drawQuad(const Internal::Surface& surface, Vector2D pos, Vector2D size, Vector2D ancor, float rotation, Graphics::QuadUV uv, float alpha)
	{
		glm::mat4 transform{ 1.0f };
		transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));
		transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::translate(transform, glm::vec3(-size.x * ancor.x, -size.y * ancor.y, 0.0f));
		drawQuad(surface, transform, size, uv, alpha);
	}

	void Render::drawFan(const Internal::Surface& surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha)
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface.bind();

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		for (const DrawPoint& point : points)
		{
			glTexCoord2f(point.texturePoint.x, point.texturePoint.y);
			glVertex2f(point.spacePoint.x, point.spacePoint.y);
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}

	void Render::drawStrip(const Internal::Surface& surface, const std::vector<DrawPoint>& points, const glm::mat4& transform, float alpha)
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface.bind();

		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		for (const DrawPoint& point : points)
		{
			glTexCoord2f(point.texturePoint.x, point.texturePoint.y);
			glVertex2f(point.spacePoint.x, point.spacePoint.y);
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}

	void Render::drawTiledQuad(const Internal::Surface& surface, Vector2D start, Vector2D size, Vector2D tiles, Vector2D uvShift)
	{
		glm::mat4 transform{ 1.0f };
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface.bind();

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(uvShift.x, uvShift.y + tiles.y); glVertex2f(start.x, start.y + size.y);
		glTexCoord2f(uvShift.x + tiles.x, uvShift.y + tiles.y); glVertex2f(start.x + size.x, start.y + size.y);
		glTexCoord2f(uvShift.x + tiles.x, uvShift.y); glVertex2f(start.x + size.x, start.y);
		glTexCoord2f(uvShift.x, uvShift.y); glVertex2f(start.x, start.y);
		glEnd();
	}

	void Renderer::renderText(const Graphics::Font& /*font*/, Vector2D /*pos*/, Graphics::Color /*color*/, const char* /*text*/)
	{
		//FC_DrawColor(font.getRawFont(), mRenderer, pos.x, pos.y, { color.R, color.G, color.B, color.A }, text);
	}

	std::array<int, 2> Renderer::getTextSize(const Graphics::Font& /*font*/, const char* /*text*/)
	{
		//return { FC_GetWidth(font.getRawFont(), text), FC_GetHeight(font.getRawFont(), text) };
		return { 1, 1 };
	}
}
