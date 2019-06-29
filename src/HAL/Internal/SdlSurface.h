#pragma once

#include <memory>
#include <string>

struct SDL_Surface;

namespace HAL
{
	namespace Internal
	{
		class SdlSurface
		{
		public:
			SdlSurface(const std::string& filename);
			operator SDL_Surface*();
			void bind();
			int width() const;
			int height() const;

		private:
			static void deleteTexture(unsigned int* textureId);
			std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> mSurface;
			std::unique_ptr<unsigned int, void(*)(unsigned int*)> mTextureId;
		};
	}
}