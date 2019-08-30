#include "HAL/Graphics/Texture.h"

#include "Debug/Assert.h"

#include "SDL.h"
#include "HAL/Internal/SdlSurface.h"

namespace Graphics
{
	Texture::Texture(const std::string& path, SDL_Renderer* renderer)
	{
		HAL::Internal::Surface tempSurface(path.c_str());
		mTexture = SDL_CreateTextureFromSurface(renderer, tempSurface.getRawSurface());
		AssertFatal(mTexture, "Texture can't be created from SdlSurface");
		mWidth = tempSurface.getWidth();
		mHeight = tempSurface.getHeight();
	}

	Texture::~Texture()
	{
		SDL_DestroyTexture(mTexture);
	}

	bool Texture::isValid() const
	{
		return mTexture != nullptr;
	}

	int Texture::getWidth() const
	{
		return mWidth;
	}

	int Texture::getHeight() const
	{
		return mHeight;
	}

	SDL_Texture* Texture::getRawTexture() const
	{
		return mTexture;
	}
}
