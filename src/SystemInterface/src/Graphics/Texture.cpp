#include "Graphics/Texture.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"

namespace Graphics
{
	Texture::Texture(SystemInterface::Internal::SdlSurface* surface)
		: mSurface(surface)
	{
	}

	int Texture::getHeight() const
	{
		return mSurface->height();
	}

	int Texture::getWidth() const
	{
		return mSurface->width();
	}

	SystemInterface::Internal::SdlSurface* Texture::getSurface() const
	{
		return mSurface;
	}

	bool Texture::isValid() const
	{
		return mSurface != nullptr;
	}
}
