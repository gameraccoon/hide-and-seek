#pragma once

#include <memory>

#include <sdl/SDL.h>

namespace HAL
{
	namespace Internal
	{
		class Window;

		class GlContext
		{
		public:
			explicit GlContext(Window& window);
			~GlContext();
			SDL_GLContext getRawGLContext();

		private:
			SDL_GLContext mContext;
		};
	}
}
