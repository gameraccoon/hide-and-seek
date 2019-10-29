#include "SdlSurface.h"

#include <stdexcept>
#include <string>
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

#include "Base/Debug/Assert.h"

namespace HAL
{
	namespace Internal
	{
		Surface::Surface(const std::string& filename)
			: mSurface(IMG_Load(filename.c_str()))
		{
			AssertFatal(mSurface, "Unable to load texture %s", filename.c_str());
		}

		Surface::~Surface()
		{
			SDL_FreeSurface(mSurface);
		}

		int Surface::getWidth() const
		{
			return mSurface->w;
		}

		int Surface::getHeight() const
		{
			return mSurface->h;
		}

		SDL_Surface* Surface::getRawSurface()
		{
			return mSurface;
		}
	}
}
