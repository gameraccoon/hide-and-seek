#include "Engine.h"

#include <stdexcept>
#include <algorithm>
#include <vector>

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <sdl/Sdl.h>

#include "Font.h"
#include "GlContext.h"
#include "Sdl.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "../IGame.h"


namespace Engine
{
	namespace Internal
	{
		static const int WindowWidth = 800;
		static const int WindowHeight = 600;
		static const float MaxFrameTicks = 300.0f;
		static const float TextScale = 0.5f;

		struct Engine::Impl
		{
			Sdl mSdl;
			SdlWindow mSdlWindow;
			GlContext mGlContext;
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
				, mMouseX(WindowWidth * 0.5f)
				, mMouseY(WindowHeight * 0.5f)
				, mMouseButtonDown(false)
				, mQuit(false)
				, mGame(nullptr)
				, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
			{
			}

			void Start();
			void ParseEvents();
		};

		Engine::Engine()
			: mPimpl(new Impl()) {
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetSwapInterval(1);

			glEnable(GL_TEXTURE_2D);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0f, WindowWidth, WindowHeight, 0.0f, -1.0f, 1.0f);
			glMatrixMode(GL_MODELVIEW);
		}

		Engine::~Engine() {
		}

		float Engine::GetMouseX() const {
			return mPimpl->mMouseX;
		}

		float Engine::GetMouseY() const {
			return mPimpl->mMouseY;
		}

		bool Engine::GetMouseButtonDown() const {
			return mPimpl->mMouseButtonDown;
		}

		void Engine::Quit() {
			mPimpl->mQuit = true;
		}

		void Engine::Start(IGame& game)
		{
			mPimpl->mGame = &game;
			mPimpl->mSdlWindow.Show();
			mPimpl->Start();
		}

		void Engine::Render(SdlSurface* surface, const glm::mat4& transform, float alpha)
		{
			glLoadMatrixf(reinterpret_cast<const float*>(&transform));
			surface->Bind();

			glBegin(GL_QUADS);
			if (alpha != 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, alpha);
			}
			glTexCoord2i(0, 1); glVertex2i(0, surface->Height());
			glTexCoord2i(1, 1); glVertex2i(surface->Width(), surface->Height());
			glTexCoord2i(1, 0); glVertex2i(surface->Width(), 0);
			glTexCoord2i(0, 0); glVertex2i(0, 0);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glEnd();
		}

		void Engine::Render(SdlSurface* surface, float x, float y, float rotation, float alpha) {
			glm::mat4 transformation;
			transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
			if (rotation) {
				transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			Render(surface, transformation, alpha);
		}

		Glyph& FindGlyph(char c) {
			auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
			if (found == std::end(Font) || c < *found) {
				found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
			}
			return *found;
		}

		float Engine::CalculateStringWidth(const char* text) const {
			int advance = 0;
			for (; *text; ++text) {
				Glyph& g = FindGlyph(*text);
				advance += g.advance;
			}
			return advance*TextScale;
		}

		void Engine::Write(SdlSurface* fontSurface, const char* text, const glm::mat4& transform) {
			glLoadMatrixf(reinterpret_cast<const float*>(&transform));
			int advance = 0;
			for (; *text; ++text) {
				Glyph& g = FindGlyph(*text);

				float fontTexWidth = static_cast<float>(fontSurface->Width());
				float fontTexHeight = static_cast<float>(fontSurface->Height());

				float uvLeft = static_cast<float>(g.x) / fontTexWidth;
				float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
				float uvBottom = static_cast<float>(g.y) / fontTexHeight;
				float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

				float worldLeft = static_cast<float>(g.xoffset + advance);
				float worldRight = static_cast<float>(g.xoffset + g.width + advance);
				float worldBottom = static_cast<float>(g.yoffset);
				float worldTop = static_cast<float>(g.yoffset + g.height);

				fontSurface->Bind();

				glBegin(GL_QUADS);
				glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
				glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
				glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
				glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
				glEnd();
				advance += g.advance;
			}
		}

		void Engine::Write(SdlSurface* fontSurface, const char* text, float x, float y, float rotation) {
			glm::mat4 transformation;
			transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
			if (rotation) {
				transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
				transformation = glm::translate(transformation, glm::vec3(-CalculateStringWidth(text) / 2.0f, -20.0f, 0.0f));
			}
			Write(fontSurface, text, transformation);
		}

		int Engine::GetWidth() const {
			return WindowWidth;
		}

		int Engine::GetHeight() const {
			return WindowHeight;
		}

		void Engine::Impl::Start() {
			while (!mQuit) {
				SDL_GL_SwapWindow(mSdlWindow);
				glClear(GL_COLOR_BUFFER_BIT);

				ParseEvents();

				float currentTicks = static_cast<float>(SDL_GetTicks());
				float lastFrameTicks = currentTicks - mElapsedTicks;
				mElapsedTicks = currentTicks;

				lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
				float lastFrameSeconds = lastFrameTicks * 0.001f;

				if (mGame)
				{
					mGame->Update(lastFrameSeconds);
					mGame->Draw();
				}
			}
		}

		void Engine::Impl::ParseEvents() {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					mQuit = true;
					break;
				case SDL_KEYDOWN: {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						mQuit = true;
					}
				} break;
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
}
