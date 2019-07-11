#include "HAL/Graphics/Sprite.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	Sprite::Sprite(HAL::Internal::SdlSurface* surface, QuadUV uv)
		: mSurface(surface)
		, mUV(uv)
	{
	}

	int Sprite::getHeight() const
	{
		return mSurface->height();
	}

	int Sprite::getWidth() const
	{
		return mSurface->width();
	}

	HAL::Internal::SdlSurface* Sprite::getSurface() const
	{
		return mSurface;
	}

	bool Sprite::isValid() const
	{
		return mSurface != nullptr;
	}
}
