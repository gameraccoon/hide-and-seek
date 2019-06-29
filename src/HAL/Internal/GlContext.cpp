#include "GlContext.h"

#include "SdlWindow.h"
#include <stdexcept>
#include <string>

namespace HAL
{
	namespace Internal
	{
		GlContext::GlContext(SdlWindow& sdlWindow)
			: mContext(SDL_GL_CreateContext(sdlWindow), SDL_GL_DeleteContext)
		{
			const char* error = SDL_GetError();
			if (*error != '\0')
			{
				throw std::runtime_error(std::string("Error initialising OpenGL context: ") + error);
			}
		}

		GlContext::operator SDL_GLContext()
		{
			return mContext.get();
		}
	}
}
