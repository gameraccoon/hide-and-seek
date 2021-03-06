#include "Base/precomp.h"

#include "HAL/Base/Engine.h"

#include "Base/Debug/ConcurrentAccessDetector.h"

#include <algorithm>

#include <glew/glew.h>

#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "HAL/IGame.h"
#include "HAL/Internal/GlContext.h"
#include "HAL/Internal/Sdl.h"
#include "HAL/Internal/SdlWindow.h"

namespace HAL
{
#ifdef CONCURRENT_ACCESS_DETECTION
	ConcurrentAccessDetector gSDLAccessDetector;
#endif

	static const float MaxFrameTicks = 300.0f;

	struct Engine::Impl
	{
		Internal::SDLInstance mSdl;
		Internal::Window mWindow;
		Internal::GlContext mGlContext;
		Graphics::Renderer mRenderer;
		float mElapsedTicks;
		IGame* mGame;
		bool mQuit;

		float mMouseX;
		float mMouseY;
		SDL_Event mLastEvent;

		Impl(int windowWidth, int windowHeight)
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE)
			, mWindow(windowWidth, windowHeight)
			, mGlContext(mWindow)
			, mRenderer()
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
			, mGame(nullptr)
			, mQuit(false)
			, mMouseX(windowWidth * 0.5f)
			, mMouseY(windowHeight * 0.5f)
		{
		}

		~Impl()
		{
		}

		void start();
		void parseEvents();
	};

	Engine::Engine(int windowWidth, int windowHeight)
		: WindowWidth(windowWidth)
		, WindowHeight(windowHeight)
		, mPimpl(HS_NEW Impl(windowWidth, windowHeight))
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

		if (TTF_Init() == -1)
		{
			ReportError("TTF_Init failed");
		}

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			ReportError("TTF_Init failed");
		}
	}

	Engine::~Engine()
	{
		Mix_CloseAudio();
		TTF_Quit();
	}

	Vector2D Engine::getMousePos() const
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return Vector2D(mPimpl->mMouseX, mPimpl->mMouseY);
	}

	void Engine::quit()
	{
		mPimpl->mQuit = true;
	}

	void Engine::start(IGame* game)
	{
		mPimpl->mGame = game;
		mPimpl->mWindow.show();
		game->initResources();
		mPimpl->start();
	}

	Graphics::Renderer& Engine::getRenderer()
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return mPimpl->mRenderer;
	}

	Vector2D Engine::getWindowSize() const
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return Vector2D(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
	}

	SDL_Window* Engine::getRawWindow()
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return mPimpl->mWindow.getRawWindow();
	}

	void* Engine::getRawGlContext()
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return mPimpl->mGlContext.getRawGLContext();
	}

	SDL_Event& Engine::getLastEventRef()
	{
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		return mPimpl->mLastEvent;
	}

	void Engine::Impl::start()
	{
		while (!mQuit)
		{
			SDL_GL_SwapWindow(mWindow.getRawWindow());
			glClear(GL_COLOR_BUFFER_BIT);

			parseEvents();

			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			if (lastFrameTicks == 0)
			{
				continue;
			}

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
		DETECT_CONCURRENT_ACCESS(gSDLAccessDetector);
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_QUIT:
				mQuit = true;
				break;
			case SDL_KEYDOWN:
				mGame->setKeyboardKeyState(event.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				mGame->setKeyboardKeyState(event.key.keysym.sym, false);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mGame->setMouseKeyState(event.button.button, true);
				break;
			case SDL_MOUSEBUTTONUP:
				mGame->setMouseKeyState(event.button.button, false);
				break;
			case SDL_MOUSEMOTION:
				mMouseX = static_cast<float>(event.motion.x);
				mMouseY = static_cast<float>(event.motion.y);
				break;
			default:
				break;
			}
		}
		mLastEvent = event;
	}
}
