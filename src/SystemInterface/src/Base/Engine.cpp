#include "Base/Engine.h"

#include <stdexcept>
#include <algorithm>
#include <vector>

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
//#include <glm/glm.hpp>

#include "../Internal/FontInternal.h"
#include "../Internal/GlContext.h"
#include "../Internal/Sdl.h"
#include "../Internal/SdlWindow.h"
#include "../Internal/SdlSurface.h"
#include "IGame.h"


namespace SystemInterface
{
	static const int WindowWidth = 800;
	static const int WindowHeight = 600;
	static const float MaxFrameTicks = 300.0f;
	static const float TextScale = 0.5f;

	struct Engine::Impl
	{
		Internal::Sdl mSdl;
		Internal::SdlWindow mSdlWindow;
		Internal::GlContext mGlContext;
		float mElapsedTicks;
		IGame* mGame;
		bool mQuit;

		float mMouseX;
		float mMouseY;
		bool mMouseButtonDown;

		Impl()
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)
			, mSdlWindow(WindowWidth, WindowHeight)
			, mGlContext(mSdlWindow)
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
			, mGame(nullptr)
			, mQuit(false)
			, mMouseX(WindowWidth * 0.5f)
			, mMouseY(WindowHeight * 0.5f)
			, mMouseButtonDown(false)
		{
		}

		void start();
		void parseEvents();
	};

	Engine::Engine()
		: mPimpl(new Impl())
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(1);

		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, WindowWidth, WindowHeight, 0.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}

	Engine::~Engine()
	{
	}

	float Engine::getMouseX() const
	{
		return mPimpl->mMouseX;
	}

	float Engine::getMouseY() const
	{
		return mPimpl->mMouseY;
	}

	bool Engine::getMouseButtonDown() const
	{
		return mPimpl->mMouseButtonDown;
	}

	void Engine::quit()
	{
		mPimpl->mQuit = true;
	}

	void Engine::start(IGame* game)
	{
		mPimpl->mGame = game;
		mPimpl->mSdlWindow.show();
		mPimpl->start();
	}

	void Engine::render(Internal::SdlSurface* surface, const glm::mat4& transform, float alpha)
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		surface->bind();

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		glTexCoord2i(0, 1); glVertex2i(0, surface->height());
		glTexCoord2i(1, 1); glVertex2i(surface->width(), surface->height());
		glTexCoord2i(1, 0); glVertex2i(surface->width(), 0);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();
	}

	void Engine::render(Internal::SdlSurface* surface, float x, float y, float ancX, float ancY, float rotation, float alpha)
	{
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = glm::translate(transformation, glm::vec3(-surface->width()*ancX, -surface->height()*ancY, 0.0f));
		render(surface, transformation, alpha);
	}

	Internal::Glyph& findGlyph(char c)
	{
		auto found = std::lower_bound(std::begin(Internal::Font), std::end(Internal::Font), c);
		if (found == std::end(Internal::Font) || c < *found)
		{
			found = std::lower_bound(std::begin(Internal::Font), std::end(Internal::Font), static_cast<int>('_'));
		}
		return *found;
	}

	float Engine::calculateStringWidth(const char* text) const
	{
		int advance = 0;
		for (; *text; ++text)
		{
			Internal::Glyph& g = findGlyph(*text);
			advance += g.advance;
		}
		return advance*TextScale;
	}

	void Engine::write(Internal::SdlSurface* fontSurface, const char* text, const glm::mat4& transform)
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		int advance = 0;
		for (; *text; ++text)
		{
			Internal::Glyph& g = findGlyph(*text);

			float fontTexWidth = static_cast<float>(fontSurface->width());
			float fontTexHeight = static_cast<float>(fontSurface->height());

			float uvLeft = static_cast<float>(g.x) / fontTexWidth;
			float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
			float uvBottom = static_cast<float>(g.y) / fontTexHeight;
			float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

			float worldLeft = static_cast<float>(g.xoffset + advance);
			float worldRight = static_cast<float>(g.xoffset + g.width + advance);
			float worldBottom = static_cast<float>(g.yoffset);
			float worldTop = static_cast<float>(g.yoffset + g.height);

			fontSurface->bind();

			glBegin(GL_QUADS);
			glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
			glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
			glEnd();
			advance += g.advance;
		}
	}

	void Engine::write(Internal::SdlSurface* fontSurface, const char* text, float x, float y, float rotation)
	{
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
        if (rotation > 0.001f || rotation < -0.001f)
		{
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			transformation = glm::translate(transformation, glm::vec3(-calculateStringWidth(text) / 2.0f, -20.0f, 0.0f));
		}
		write(fontSurface, text, transformation);
	}

	int Engine::getWidth() const
	{
		return WindowWidth;
	}

	int Engine::getHeight() const
	{
		return WindowHeight;
	}

	void Engine::Impl::start()
	{
		while (!mQuit)
		{
			SDL_GL_SwapWindow(mSdlWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			parseEvents();

			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
			float lastFrameSeconds = lastFrameTicks * 0.001f;

			if (mGame)
			{
				mGame->update(lastFrameSeconds);
			}
		}
	}

	void Engine::Impl::parseEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_QUIT:
				mQuit = true;
				break;
			case SDL_KEYDOWN:
				mGame->setKeyState(event.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				mGame->setKeyState(event.key.keysym.sym, false);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mMouseButtonDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mMouseButtonDown = false;
				break;
			case SDL_MOUSEMOTION:
				mMouseX = static_cast<float>(event.motion.x);
				mMouseY = static_cast<float>(event.motion.y);
				break;
			default:
				break;
			}
		}
	}
}
