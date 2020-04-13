#include "Base/precomp.h"

#include "SdlSurface.h"

#include <stdexcept>
#include <string>
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_surface.h>
#include <sdl/SDL_image.h>

namespace Graphics
{
	namespace Internal
	{
		Surface::Surface(const std::string& filename)
			: mSurface(IMG_Load(filename.c_str()))
			, mTextureID(0)
		{
			AssertFatal(mSurface, "Unable to load texture %s", filename);

			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);

			int mode;
			switch (mSurface->format->BytesPerPixel)
			{
			case 4:
				mode = GL_RGBA;
				break;
			case 3:
				mode = GL_RGB;
				break;
			case 1:
				mode = GL_LUMINANCE_ALPHA;
				break;
			default:
				throw std::runtime_error("Image with unknown channel profile");
			}

			glTexImage2D(GL_TEXTURE_2D, 0, mode, mSurface->w, mSurface->h, 0, static_cast<GLenum>(mode), GL_UNSIGNED_BYTE, mSurface->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		Surface::~Surface()
		{
			SDL_FreeSurface(mSurface);
			glDeleteTextures(1, &mTextureID);
		}

		int Surface::getWidth() const
		{
			return mSurface->w;
		}

		int Surface::getHeight() const
		{
			return mSurface->h;
		}

		void Surface::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, mTextureID);
		}

		bool Surface::isValid() const
		{
			return mSurface != nullptr;
		}
	}
}
