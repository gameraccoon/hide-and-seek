#pragma once

#include <memory>

struct SDL_Surface;

namespace SystemInterface
{
	namespace Internal
	{
		class SdlSurface
		{
		public:
			SdlSurface(const char* filename);
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
