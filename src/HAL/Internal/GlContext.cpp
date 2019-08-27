#include "GlContext.h"

#include "SdlWindow.h"
#include <stdexcept>
#include <string>

namespace HAL
{
	namespace Internal
	{
		GlContext::GlContext(Window& window)
			: mContext(SDL_GL_CreateContext(window.getRawWindow()))
		{
			const char* error = SDL_GetError();
			if (*error != '\0')
			{
				throw std::runtime_error(std::string("Error initialising OpenGL context: ") + error);
			}
		}

		GlContext::~GlContext()
		{
			SDL_GL_DeleteContext(mContext);
		}

		SDL_GLContext GlContext::getRawGLContext()
		{
			return mContext;
		}
	}
}
