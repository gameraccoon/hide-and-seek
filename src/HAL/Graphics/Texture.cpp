#include "HAL/Graphics/Texture.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	Texture::Texture(HAL::Internal::SdlSurface* surface)
		: mSurface(surface)
	{
	}

	HAL::Internal::SdlSurface* Texture::getSurface() const
	{
		return mSurface;
	}

	bool Texture::isValid() const
	{
		return mSurface != nullptr;
	}
}
